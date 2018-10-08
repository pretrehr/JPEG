#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <structures.h>


#define min(a,b) (a<=b?a:b)


/***************************************

Redimensionnement d'une image en nuance de gris en une image de taille multiple de 8

***************************************/

struct image redim_gris(struct image img)
{
  if (((img.hauteur)%8==0) & ((img.largeur)%8==0))
  {
    return img;
  }
  uint16_t nouv_h = (img.hauteur+7)/8*8;
  uint16_t nouv_l = (img.largeur+7)/8*8;
  uint8_t *tab = malloc(nouv_h * nouv_l * sizeof(uint8_t));
  for (int i=0; i<nouv_h; i++)
  {
    for (int j=0; j<nouv_l; j++)
    {
      tab[j+nouv_l*i] = img.pixel[min(j,img.largeur-1) + img.largeur*min(i,img.hauteur-1)];
    }
  }

  struct image nv_image = {nouv_l, nouv_h, img.couleur, tab};
  return nv_image;
}


/***************************************

Redimensionnement d'une image en nuance de gris en une image de taille multiple de h1 et v1

***************************************/


struct image redim_couleur(struct image img, uint8_t h1, uint8_t v1)
{
  uint16_t h_mcu = 8*v1, l_mcu = 8*h1;
  if (((img.hauteur)%h_mcu==0) && ((img.largeur)%l_mcu==0))
  {
    return img;
  }
  uint16_t nouv_h = (img.hauteur+h_mcu-1)/h_mcu*h_mcu;
  uint16_t nouv_l = (img.largeur+l_mcu-1)/l_mcu*l_mcu;
  uint8_t *tab = malloc(3 * nouv_h * nouv_l * sizeof(uint8_t));
  for (int i=0; i<nouv_h; i++)
  {
    for (int j=0; j<nouv_l; j++)
    {
      for (int k=0; k<3; k++)
      {
        tab[k+j*3+3*nouv_l*i] = img.pixel[k + 3*min(j,img.largeur-1) + 3*img.largeur*min(i, img.hauteur-1)];
      }
    }
  }
  struct image nv_image = {nouv_l, nouv_h, img.couleur, tab};
  return nv_image;
}
