#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <structures.h>



/***************************************

Conversion d'une image RGB en YCbCr

***************************************/

struct imageycbcr8 ycbcr(struct image img)
{
  uint16_t h = img.hauteur, l = img.largeur;
  uint32_t nb_blocs = h*l/64;
  uint16_t i_pixel, j_pixel, i_pixel_dans_bloc, j_pixel_dans_bloc, i_bloc, j_bloc;
  uint32_t k;
  struct imageycbcr8 image_out;
  if (img.couleur == 0)
  {
    uint8_t **y = calloc(nb_blocs,sizeof(uint8_t *));
    uint8_t **cb = NULL;
    uint8_t **cr = NULL;
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      y[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
    }
    image_out.y = y;
    image_out.cb = cb;
    image_out.cr = cr;
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      for (int j=0; j<64; j++)
      {
        i_pixel_dans_bloc = j/8;
        j_pixel_dans_bloc = j%8;
        i_bloc = i/(l/8);
        j_bloc = i%(l/8);
        i_pixel = i_bloc*8+i_pixel_dans_bloc;
        j_pixel = j_bloc*8+j_pixel_dans_bloc;
        k = l*i_pixel + j_pixel;
        image_out.y[i][j] = img.pixel[k];
      }
    }
    image_out.largeur = l;
    image_out.hauteur = h;
    image_out.couleur = 0;
    return image_out;
  }
  uint8_t **y = (uint8_t **)calloc(nb_blocs,sizeof(uint8_t *));
  uint8_t **cb = (uint8_t **)calloc(nb_blocs,sizeof(uint8_t *));
  uint8_t **cr = (uint8_t **)calloc(nb_blocs,sizeof(uint8_t *));
  for (uint32_t i=0; i<nb_blocs; i++)
  {
    y[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
    cb[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
    cr[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
  }
  image_out.y = y;
  image_out.cr = cb;
  image_out.cb = cr;
  for (uint32_t i=0; i<nb_blocs; i++)
  {
    for (int j=0; j<64; j++)
    {
      i_pixel_dans_bloc = j/8;
      j_pixel_dans_bloc = j%8;
      i_bloc = i/(l/8);
      j_bloc = i%(l/8);
      i_pixel = i_bloc*8+i_pixel_dans_bloc;
      j_pixel = j_bloc*8+j_pixel_dans_bloc;
      k = l*i_pixel + j_pixel;
      image_out.y[i][j] = 0.299*(float)img.pixel[3*k]+0.587*(float)img.pixel[3*k+1]+0.114*(float)img.pixel[3*k+2];
      image_out.cb[i][j] = -0.1687*(float)img.pixel[3*k]-0.3313*(float)img.pixel[3*k+1]+0.5*(float)img.pixel[3*k+2]+128;
      image_out.cr[i][j] = 0.5*(float)img.pixel[3*k]-0.4187*(float)img.pixel[3*k+1]-0.0813*(float)img.pixel[3*k+2]+128;
    }
  }
  image_out.largeur = img.largeur;
  image_out.hauteur = img.hauteur;
  image_out.couleur = 1;
  return image_out;
}

#ifdef TEST_YCBCR
int main()
{
  uint8_t *tab = malloc(64*sizeof(uint8_t));
  for (int i=0; i<64; i++)
  {
    tab[i] = i;
  }
  struct image img = {8,8,0,tab};
  struct imageycbcr8 img2 = ycbcr(img);
  for (int i=0; i<64; i++)
  {
    printf("%d ", img2.y[0][i]);
  }
  uint8_t *tab2 = malloc(192*sizeof(uint8_t));
  for (int i=0; i<192; i++)
  {
    tab2[i] = i/3;
  }
  struct image img3 = {8,8,1,tab2};
  struct imageycbcr8 img4 = ycbcr(img3);
  for (int i=0; i<64; i++)
  {
    printf("%d ", img4.y[0][i]);
  }
  return 0;
}
#endif
