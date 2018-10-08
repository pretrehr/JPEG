#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <compression.h>
#include <bitstream.h>
#include <huffman.h>
#include <htables.h>
#include <qtables.h>
#include <lecture_ecriture.h>
#include <jpeg_writer.h>
#include <structures.h>
#include <iteration_mcu.h>
#include <fonction_bloc.h>



#ifdef TEST

int main(){

    printf("JE SUIS UN TEST");

    struct image grey = lecture_image("complexite.pgm");

    grey = redim_gris(grey);

    struct imageycbcr8 grey2 = ycbcr(grey);

    image_destroy(grey);

    struct imageycbcr16 grey3 = f_image8_16(grey2, dct_rapide);

    imageycbcr8_destroy(grey2);
    grey3 = f_image16_16(grey3, zigzag);
    grey3 = f_image16_16(grey3, quantif_y);

    uint16_t hauteur = grey3.hauteur;

    uint16_t largeur = grey3.largeur;

    struct huff_table *table_ac_y = huffman_table_build(htables_nb_symb_per_lengths[AC][Y],
            htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);


    struct huff_table *table_dc_y = huffman_table_build(htables_nb_symb_per_lengths[DC][Y],
            htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);

    uint8_t sampling[2] = {1,1};


    struct jpeg_desc *jpeg = entete("complexite.pgm", "complexite.jpeg", hauteur, largeur, 1, sampling, table_ac_y, table_dc_y, NULL, NULL, NULL, NULL, compressed_Y_table, compressed_CbCr_table);


    jpeg_write_header(jpeg);
    struct bitstream *stream = jpeg_desc_get_bitstream(jpeg);

    bitstream_flush(stream);
    uint8_t nbits = 0;

    int16_t dc = 0;
    uint32_t n = largeur*hauteur/64;
    for (uint32_t i = 0; i < n; i++){
        encode(stream, grey3.y[i], &dc, table_dc_y, table_ac_y, &nbits);
    }
    imageycbcr16_destroy(grey3);

    bitstream_flush(stream);

    jpeg_write_footer(jpeg);

    bitstream_destroy(stream);
}

#endif
