#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <structures.h>
#include <erreurs.h>

/***************************************

Effectue un arrondi a l'entier le plus proche

***************************************/

uint8_t arrondi(float x)
{
  uint8_t y = (uint8_t)(x+0.5);
  return y;
}


/***************************************

Decoupe une image de type RGB en MCUs de YCbCr

***************************************/



struct imageycbcr8 decoupe_mcu(struct image img, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3)
{
  uint16_t h = img.hauteur, l = img.largeur;
  uint32_t nb_MCUs = h*l/(h1*v1*64);
  uint16_t i_pixel, j_pixel, i_pixel_dans_MCU, j_pixel_dans_MCU, i_MCU, j_MCU;
  uint32_t k;
  uint16_t r_moyen = 0, g_moyen = 0, b_moyen = 0;
  struct imageycbcr8 image_out;
  uint8_t **y = malloc(nb_MCUs * sizeof(uint8_t *));
  uint8_t **cb = malloc(nb_MCUs * sizeof(uint8_t *));
  uint8_t **cr = malloc(nb_MCUs * sizeof(uint8_t *));
  for (uint32_t i=0; i<nb_MCUs; i++)
  {
    y[i] = malloc(h1*v1*64 * sizeof(uint8_t));
    cb[i] = malloc(h2*v2*64 * sizeof(uint8_t));
    cr[i] = malloc(h3*v3*64 * sizeof(uint8_t));
  }
  for (uint32_t i=0; i<nb_MCUs; i++)
  {
    for (int j=0; j<64*h1*v1; j++)
    {
      i_pixel_dans_MCU = j/(8*h1);
      j_pixel_dans_MCU = j%(8*h1);
      i_MCU = i/(l/(8*h1));
      j_MCU = i%(l/(8*h1));
      i_pixel = i_MCU*8*v1+i_pixel_dans_MCU;
      j_pixel = j_MCU*8*h1+j_pixel_dans_MCU;
      k = l*i_pixel + j_pixel;
      y[i][j] = arrondi(0.299*(float)img.pixel[3*k]+0.587*(float)img.pixel[3*k+1]+0.114*(float)img.pixel[3*k+2]);
    }
  }
  for (uint32_t i=0; i<nb_MCUs; i++)
  {
    for (int j=0; j<h2*v2*64; j++)
    {
      i_pixel_dans_MCU = j/(8*h2);
      j_pixel_dans_MCU = j%(8*h2);
      i_MCU = i/(l/(8*h1));
      j_MCU = i%(l/(8*h1));
      i_pixel = (i_MCU*8*v2+i_pixel_dans_MCU)*v1/v2;
      j_pixel = (j_MCU*8*h2+j_pixel_dans_MCU)*h1/h2;
      k = l*i_pixel + j_pixel;
      r_moyen = 0;
      g_moyen = 0;
      b_moyen = 0;
      for (int i_h = 0; i_h < v1/v2; i_h++)
      {
        for (int i_v = 0; i_v < h1/h2; i_v++)
        {
          r_moyen += img.pixel[3*(k+i_v+l*i_h)];
          g_moyen += img.pixel[3*(k+i_v+l*i_h)+1];
          b_moyen += img.pixel[3*(k+i_v+l*i_h)+2];
        }
      }

      cb[i][j] = arrondi((-0.1687*(float)r_moyen-0.3313*(float)g_moyen+0.5*(float)b_moyen)/((float)(v1/v2*h1/h2))+128);
    }
  }

  for (uint32_t i=0; i<nb_MCUs; i++)
  {
    for (int j=0; j<h3*v3*64; j++)
    {
      i_pixel_dans_MCU = j/(8*h3);
      j_pixel_dans_MCU = j%(8*h3);
      i_MCU = i/(l/(8*h1));
      j_MCU = i%(l/(8*h1));
      i_pixel = (i_MCU*8*v3+i_pixel_dans_MCU)*v1/v3;
      j_pixel = (j_MCU*8*h3+j_pixel_dans_MCU)*h1/h3;
      k = l*i_pixel + j_pixel;
      r_moyen = 0;
      g_moyen = 0;
      b_moyen = 0;
      for (int i_h = 0; i_h < v1/v3; i_h++)
      {
        for (int i_v = 0; i_v < h1/h3; i_v++)
        {
          r_moyen += img.pixel[3*(k+i_v+l*i_h)];
          g_moyen += img.pixel[3*(k+i_v+l*i_h)+1];
          b_moyen += img.pixel[3*(k+i_v+l*i_h)+2];
        }
      }

      cr[i][j] = arrondi((0.5*(float)r_moyen-0.4187*(float)g_moyen-0.0813*(float)b_moyen)/((float)(v1/v3*h1/h3))+128);
    }
  }
  image_out.y = y;
  image_out.cb = cb;
  image_out.cr = cr;
  image_out.largeur = img.largeur;
  image_out.hauteur = img.hauteur;
  image_out.couleur = 1;

  return image_out;
}



/***************************************

Decoupe une image composee de MCUs en blocs 8x8

***************************************/


struct imageycbcr8 decoupe_suiv(struct imageycbcr8 img, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3)
{
  uint16_t h = img.hauteur, l = img.largeur;
  uint32_t nb_blocs_y = h*l/64;
  uint32_t nb_blocs_cb = h*l*h2*v2/(64*h1*v1);
  uint32_t nb_blocs_cr = h*l*h3*v3/(64*h1*v1);
  uint16_t i_pixel, j_pixel, i_pixel_dans_bloc, j_pixel_dans_bloc, i_bloc, j_bloc;
  uint32_t k_MCU, k_pixel;
  struct imageycbcr8 image_out;
  uint8_t **y = (uint8_t **)calloc(nb_blocs_y,sizeof(uint8_t *));
  uint8_t **cb = (uint8_t **)calloc(nb_blocs_cb,sizeof(uint8_t *));
  uint8_t **cr = (uint8_t **)calloc(nb_blocs_cr,sizeof(uint8_t *));

  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    y[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    cb[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    cr[i] = (uint8_t *)calloc(64,sizeof(uint8_t));
  }
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    for (int j=0; j<64; j++)
    {
      k_MCU = i/(h1*v1);
      i_pixel_dans_bloc = j/8;
      j_pixel_dans_bloc = j%8;
      i_bloc = i/h1;
      j_bloc = i%h1;
      i_pixel = i_bloc*8+i_pixel_dans_bloc;
      j_pixel = j_bloc*8+j_pixel_dans_bloc;
      k_pixel = (8*h1*i_pixel + j_pixel)%(64*v1*h1);
      y[i][j] = img.y[k_MCU][k_pixel];
    }
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    for (int j=0; j<64; j++)
    {
      k_MCU = i/(h2*v2);
      i_pixel_dans_bloc = j/8;
      j_pixel_dans_bloc = j%8;
      i_bloc = i/h2;
      j_bloc = i%h2;
      i_pixel = i_bloc*8+i_pixel_dans_bloc;
      j_pixel = j_bloc*8+j_pixel_dans_bloc;
      k_pixel = (8*h2*i_pixel + j_pixel)%(64*v2*h2);
      cb[i][j] = img.cb[k_MCU][k_pixel];
    }
  }

  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    for (int j=0; j<64; j++)
    {
      k_MCU = i/(h3*v3);
      i_pixel_dans_bloc = j/8;
      j_pixel_dans_bloc = j%8;
      i_bloc = i/h3;
      j_bloc = i%h3;
      i_pixel = i_bloc*8+i_pixel_dans_bloc;
      j_pixel = j_bloc*8+j_pixel_dans_bloc;
      k_pixel = (8*h3*i_pixel + j_pixel)%(64*v3*h3);
      cr[i][j] = img.cr[k_MCU][k_pixel];
    }
  }

  image_out.y = y;
  image_out.cb = cb;
  image_out.cr = cr;
  image_out.largeur = img.largeur;
  image_out.hauteur = img.hauteur;
  image_out.couleur = 1;
  uint32_t nb_MCUs = h*l/(h1*v1*64);
  for (uint32_t i = 0; i<nb_MCUs; i++)
  {
    free(img.y[i]);
    free(img.cb[i]);
    free(img.cr[i]);
  }
  free(img.y);
  free(img.cb);
  free(img.cr);
  return image_out;
}
