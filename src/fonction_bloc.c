#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <compression.h>
#include <structures.h>


/***************************************

Libere l'espace alloue a une matrice bloc

***************************************/

void matrix_destroy16(int16_t **ptr, int nb_blocs)
{
  for (int i = 0; i < nb_blocs; i++)
  {
    free(ptr[i]);
  }
  free(ptr);
}

/***************************************

Libere totalement l'espace alloue a une image YCbCr

***************************************/

void imageycbcr16_destroy_complet(struct imageycbcr16 img, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3){
    uint32_t nb_blocs_y = img.hauteur*img.largeur/64;
    matrix_destroy16(img.y, nb_blocs_y);
    if (img.couleur == 1)
    {
      uint32_t nb_blocs_cb = img.hauteur*img.largeur*h2*v2/(64*h1*v1);
      uint32_t nb_blocs_cr = img.hauteur*img.largeur*h3*v3/(64*h1*v1);
      matrix_destroy16(img.cb, nb_blocs_cb);
      matrix_destroy16(img.cr, nb_blocs_cr);
    }
}


/***************************************

Libere l'espace alloue aux composantes Y, Cb et Cr

***************************************/

void imageycbcr8_destroy(struct imageycbcr8 img){
    free(img.y);
    free(img.cb);
    free(img.cr);
}

/***************************************

Libere l'espace alloue aux composantes Y, Cb et Cr

***************************************/

void imageycbcr16_destroy(struct imageycbcr16 img){
    free(img.y);
    free(img.cb);
    free(img.cr);
}

/***************************************

Libere totalement l'espace alloue a une image RGB

***************************************/

void image_destroy(struct image img){
    free(img.pixel);
}


/***************************************

Effectue l'operation f(bloc) a tous les blocs des 3 composantes d'une image

***************************************/

struct imageycbcr16 f_image8_16(struct imageycbcr8 img, int16_t* (*f)(uint8_t[64]), uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3)
{
  uint16_t largeur = img.largeur;
  uint16_t hauteur = img.hauteur;
  uint32_t nb_blocs = hauteur*largeur/64;
  if (img.couleur == 0)
  {
    int16_t **out = (int16_t **)calloc(nb_blocs,sizeof(int16_t *));
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      out[i] = (int16_t *)calloc(64,sizeof(int16_t));
    }
    uint8_t *pointeur;
    int16_t *tab;
    for (uint32_t i=0; i<nb_blocs; i++)
     {
      pointeur = img.y[i];
      tab = f(pointeur);
      for (int j=0; j<64; j++)
      {
        out[i][j] = tab[j];
      }
      free(tab);
    }
    struct imageycbcr16 image_out = {largeur, hauteur, 0, out, NULL, NULL};
    return image_out;
  }
  uint32_t nb_blocs_y = hauteur*largeur/64;
  uint32_t nb_blocs_cb = hauteur*largeur*h2*v2/(64*h1*v1);
  uint32_t nb_blocs_cr = hauteur*largeur*h3*v3/(64*h1*v1);
  int16_t **outy = (int16_t **)calloc(nb_blocs_y,sizeof(int16_t *));
  int16_t **outcb = (int16_t **)calloc(nb_blocs_cb,sizeof(int16_t *));
  int16_t **outcr = (int16_t **)calloc(nb_blocs_cr,sizeof(int16_t *));
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    outy[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    outcb[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    outcr[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  uint8_t *pointeury, *pointeurcb, *pointeurcr;
  int16_t *taby, *tabcb, *tabcr;
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    pointeury = img.y[i];
    taby = f(pointeury);
    for (int j=0; j<64; j++)
    {
      outy[i][j] = taby[j];
    }
    free(taby);
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    pointeurcb = img.cb[i];
    tabcb = f(pointeurcb);
    for (int j=0; j<64; j++)
    {
      outcb[i][j] = tabcb[j];
    }
    free(tabcb);
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    pointeurcr = img.cr[i];
    tabcr = f(pointeurcr);
    for (int j=0; j<64; j++)
    {
      outcr[i][j] = tabcr[j];
    }
    free(tabcr);
  }
  struct imageycbcr16 image_out;
  image_out.hauteur = hauteur;
  image_out.largeur = largeur;
  image_out.couleur = 1;
  image_out.y = outy;
  image_out.cb = outcb;
  image_out.cr = outcr;
  return image_out;
}



/***************************************

Effectue l'operation f(bloc) a tous les blocs des 3 composantes d'une image

***************************************/

struct imageycbcr16 f_image16_16(struct imageycbcr16 img, int16_t* (*f)(int16_t[64]), uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3)
{
  uint16_t largeur = img.largeur;
  uint16_t hauteur = img.hauteur;
  uint32_t nb_blocs = hauteur*largeur/64;
  if (img.couleur == 0)
  {
    int16_t **out = (int16_t **)calloc(nb_blocs,sizeof(int16_t *));
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      out[i] = (int16_t *)calloc(64,sizeof(int16_t));
    }
    int16_t *pointeur;
    int16_t *tab;
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      pointeur = img.y[i];
      tab = f(pointeur);
      for (int j=0; j<64; j++)
      {
        out[i][j] = tab[j];
      }
      free(tab);
    }
    struct imageycbcr16 image_out;
    image_out.hauteur = hauteur;
    image_out.largeur = largeur;
    image_out.couleur = 0;
    image_out.y = out;
    image_out.cb = NULL;
    image_out.cr = NULL;
    return image_out;
  }
  uint32_t nb_blocs_y = hauteur*largeur/64;
  uint32_t nb_blocs_cb = hauteur*largeur*h2*v2/(64*h1*v1);
  uint32_t nb_blocs_cr = hauteur*largeur*h3*v3/(64*h1*v1);
  int16_t **outy = (int16_t **)calloc(nb_blocs_y,sizeof(int16_t *));
  int16_t **outcb = (int16_t **)calloc(nb_blocs_cb,sizeof(int16_t *));
  int16_t **outcr = (int16_t **)calloc(nb_blocs_cr,sizeof(int16_t *));
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    outy[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    outcb[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    outcr[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  int16_t *pointeury, *pointeurcb, *pointeurcr;
  int16_t *taby, *tabcb, *tabcr;
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    pointeury = img.y[i];
    taby = f(pointeury);
    for (int j=0; j<64; j++)
    {
      outy[i][j] = taby[j];
    }
    free(taby);
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    pointeurcb = img.cb[i];
    tabcb = f(pointeurcb);
    for (int j=0; j<64; j++)
    {
      outcb[i][j] = tabcb[j];
    }
    free(tabcb);
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    pointeurcr = img.cr[i];
    tabcr = f(pointeurcr);
    for (int j=0; j<64; j++)
    {
      outcr[i][j] = tabcr[j];
    }
    free(tabcr);
  }
  struct imageycbcr16 image_out;
  image_out.hauteur = hauteur;
  image_out.largeur = largeur;
  image_out.couleur = 1;
  image_out.y = outy;
  image_out.cb = outcb;
  image_out.cr = outcr;
  return image_out;
}



/***************************************

Effectue l'operation f(bloc) a tous les blocs de la composante Y d'une image
et g(bloc) a tous les blocs des composantes Cb et Cr d'une image

***************************************/


struct imageycbcr16 f_g_image16_16(struct imageycbcr16 img, int16_t* (*f)(int16_t[64]), int16_t* (*g)(int16_t[64]), uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3)
{
  uint16_t largeur = img.largeur;
  uint16_t hauteur = img.hauteur;
  uint32_t nb_blocs = hauteur*largeur/64;
  if (img.couleur == 0)
  {
    int16_t **out = (int16_t **)calloc(nb_blocs,sizeof(int16_t *));
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      out[i] = (int16_t *)calloc(64,sizeof(int16_t));
    }
    int16_t *pointeur;
    int16_t *tab;
    for (uint32_t i=0; i<nb_blocs; i++)
    {
      pointeur = img.y[i];
      tab = f(pointeur);
      for (int j=0; j<64; j++)
      {
        out[i][j] = tab[j];
      }
      free(tab);
    }
    struct imageycbcr16 image_out;
    image_out.hauteur = hauteur;
    image_out.largeur = largeur;
    image_out.couleur = 0;
    image_out.y = out;
    image_out.cb = NULL;
    image_out.cr = NULL;
    return image_out;
  }
  uint32_t nb_blocs_y = hauteur*largeur/64;
  uint32_t nb_blocs_cb = hauteur*largeur*h2*v2/(64*h1*v1);
  uint32_t nb_blocs_cr = hauteur*largeur*h3*v3/(64*h1*v1);
  int16_t **outy = (int16_t **)calloc(nb_blocs_y,sizeof(int16_t *));
  int16_t **outcb = (int16_t **)calloc(nb_blocs_cb,sizeof(int16_t *));
  int16_t **outcr = (int16_t **)calloc(nb_blocs_cr,sizeof(int16_t *));
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    outy[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    outcb[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    outcr[i] = (int16_t *)calloc(64,sizeof(int16_t));
  }
  int16_t *pointeury, *pointeurcb, *pointeurcr;
  int16_t *taby, *tabcb, *tabcr;
  for (uint32_t i=0; i<nb_blocs_y; i++)
  {
    pointeury = img.y[i];
    taby = f(pointeury);
    for (int j=0; j<64; j++)
    {
      outy[i][j] = taby[j];
    }
    free(taby);
  }
  for (uint32_t i=0; i<nb_blocs_cb; i++)
  {
    pointeurcb = img.cb[i];
    tabcb = g(pointeurcb);
    for (int j=0; j<64; j++)
    {
      outcb[i][j] = tabcb[j];
    }
    free(tabcb);
  }
  for (uint32_t i=0; i<nb_blocs_cr; i++)
  {
    pointeurcr = img.cr[i];
    tabcr = g(pointeurcr);
    for (int j=0; j<64; j++)
    {
      outcr[i][j] = tabcr[j];
    }
    free(tabcr);
  }
  struct imageycbcr16 image_out;
  image_out.hauteur = hauteur;
  image_out.largeur = largeur;
  image_out.couleur = 1;
  image_out.y = outy;
  image_out.cb = outcb;
  image_out.cr = outcr;
  return image_out;
}
