#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <erreurs.h>
#include <compression.h>
#include <bitstream.h>
#include <huffman.h>
#include <htables.h>
#include <qtables.h>
#include <lecture_ecriture.h>
#include <jpeg_writer.h>
#include <structures.h>
#include <fonction_bloc.h>
#include <downsampling.h>

#define min(a,b) (a<=b?a:b)



int main(int argc, char **argv)
{
  bool verbose = 0;
  char *nom_jpeg = NULL;
  char *nom_ppm = NULL;
  uint8_t *h = malloc(3*sizeof(uint8_t));
  uint8_t *v = malloc(3*sizeof(uint8_t));
  for(int i = 0; i<3; i++)
  {
    h[i] = 1;
    v[i] = 1;
  }


  get_option(argc, argv, &verbose, &nom_jpeg, &nom_ppm, &h, &v);

    if (verbose) printf("Lecture_image\n");
    struct image img = lecture_image(nom_ppm, verbose, nom_jpeg, h, v);

    struct image img_redim;

    if (img.couleur == 0 && (h[0] > 1 || v[0] > 1))
    {
      free(h);
      free(v);
      free(nom_jpeg);
      image_destroy(img);
      erreur("Pas de downsampling en nuances de gris");
    }

    if(img.couleur == 0){
    if (verbose) printf("Redimensionnement gris\n");
        img_redim = redim_gris(img);
    }

    else{
    if (verbose) printf("Redimensionnement couleur\n");
        img_redim = redim_couleur(img, h[0], v[0]);
    }

    if (!(((img.hauteur)%(v[0]*8)==0) & ((img.largeur)%(h[0]*8)==0)))
    {
      image_destroy(img);
    }
    struct imageycbcr8 img2;

    if(img.couleur == 0 || (h[0] == 1 && v[0] == 1))
    {
    if (verbose) printf("Decoupage\n");
      img2 = ycbcr(img_redim);
    }
    else
    {
    if (verbose) printf("Decoupage couleur\n");
      struct imageycbcr8 img2bis = decoupe_mcu(img_redim, h[0], v[0], h[1], v[1], h[2], v[2]);
      img2 = decoupe_suiv(img2bis, h[0], v[0], h[1], v[1], h[2], v[2]);
    }

    image_destroy(img_redim);

    struct imageycbcr16 img3 = f_image8_16(img2, dct_rapide, h[0], v[0], h[1], v[1], h[2], v[2]);
    imageycbcr8_destroy(img2);

    struct imageycbcr16 img4 = f_image16_16(img3, zigzag, h[0], v[0], h[1], v[1], h[2], v[2]);
    imageycbcr16_destroy(img3);

    struct imageycbcr16 img5 = f_g_image16_16(img4, quantif_y, quantif_c, h[0], v[0], h[1], v[1], h[2], v[2]);
    imageycbcr16_destroy(img4);

    uint16_t hauteur = img5.hauteur;

    uint16_t largeur = img5.largeur;

    struct huff_table *table_ac_y = huffman_table_build(htables_nb_symb_per_lengths[AC][Y],
            htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);


    struct huff_table *table_dc_y = huffman_table_build(htables_nb_symb_per_lengths[DC][Y],
            htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);

    bool couleur = img5.couleur;
    uint8_t sampling[6] = {h[0],v[0],h[1],v[1],h[2],v[2]};

    struct jpeg_desc *jpeg;
    struct bitstream *stream;
    struct huff_table *table_ac_cb;
    struct huff_table *table_dc_cb;
    struct huff_table *table_ac_cr;
    struct huff_table *table_dc_cr;

    if(couleur != 0){
        table_ac_cb = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb],
                htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);


        table_dc_cb = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb],
                htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);

        table_ac_cr = huffman_table_build(htables_nb_symb_per_lengths[AC][Cr],
                htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]);


        table_dc_cr = huffman_table_build(htables_nb_symb_per_lengths[DC][Cr],
                htables_symbols[DC][Cr], htables_nb_symbols[DC][Cr]);

    if (verbose) printf("Entete couleur\n");
        jpeg = entete(nom_ppm, nom_jpeg, hauteur, largeur, 3, sampling, table_ac_y, table_dc_y,
                    table_ac_cb, table_dc_cb, table_ac_cr, table_ac_cr,
                    compressed_Y_table, compressed_CbCr_table);


        jpeg_write_header(jpeg);
        stream = jpeg_desc_get_bitstream(jpeg);

        bitstream_flush(stream);
        uint8_t nbits = 0;

        int16_t dc_y = 0, dc_cb = 0, dc_cr = 0;
        uint32_t nb_MCU = hauteur*largeur/(h[0]*v[0]*64);
    if (verbose) printf("Encode MCU\n");
        for (uint32_t i = 0; i < nb_MCU; i++){
          for (uint8_t j = 0; j < h[0]*v[0]; j++)
          {
            encode(stream, img5.y[i*h[0]*v[0]+j], &dc_y, table_dc_y, table_ac_y, &nbits);
          }
          for (uint8_t j = 0; j < h[1]*v[1]; j++)
          {
            encode(stream, img5.cb[i*h[1]*v[1]+j], &dc_cb, table_dc_cb, table_ac_cb, &nbits);
          }
          for (uint8_t j = 0; j < h[2]*v[2]; j++)
          {
            encode(stream, img5.cr[i*h[2]*v[2]+j], &dc_cr, table_dc_cr, table_ac_cr, &nbits);
          }
        }

    }

    else{

    if (verbose) printf("Entete gris\n");
        jpeg = entete(nom_ppm, nom_jpeg, hauteur, largeur, 1, sampling, table_ac_y, table_dc_y, NULL, NULL, NULL, NULL, compressed_Y_table, compressed_CbCr_table);


        jpeg_write_header(jpeg);
        stream = jpeg_desc_get_bitstream(jpeg);

        bitstream_flush(stream);
        uint8_t nbits = 0;

        int16_t dc = 0;
        uint32_t n = largeur*hauteur/64;
    if (verbose) printf("Encode gris\n");
        for (uint32_t i = 0; i < n; i++){
            encode(stream, img5.y[i], &dc, table_dc_y, table_ac_y, &nbits);
        }
    }

    bitstream_flush(stream);

    jpeg_write_footer(jpeg);



    if (verbose) printf("Free\n");
  huffman_table_destroy(table_dc_y);
  huffman_table_destroy(table_ac_y);
  if (couleur != 0)
  {
    huffman_table_destroy(table_dc_cb);
    huffman_table_destroy(table_ac_cb);
    huffman_table_destroy(table_dc_cr);
    huffman_table_destroy(table_ac_cr);
  }
  imageycbcr16_destroy_complet(img5, h[0], v[0], h[1], v[1], h[2], v[2]);
  jpeg_desc_destroy(jpeg);
  if(verbose){
      FILE* fichier = fopen(nom_jpeg, "r");
      fseek(fichier, 0, SEEK_END);
      uint32_t taille = ftell(fichier);
      printf("La taille du fichier compressé est : %i\n", taille);
      fclose(fichier);
  }

  free(nom_jpeg);
  free(h);
  free(v);
  return 0;
}
