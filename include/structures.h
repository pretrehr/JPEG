#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdint.h>
#include <stdbool.h>

struct image
{
  uint16_t largeur;
  uint16_t hauteur;
  bool couleur;
  uint8_t *pixel;
};

// struct imagergb
// {
//   uint16_t largeur;
//   uint16_t hauteur;
//   bool couleur;
//   uint8_t **r;
//   uint8_t **g;
//   uint8_t **b;
// };

struct imageycbcr8
{
  uint16_t largeur;
  uint16_t hauteur;
  bool couleur;
  uint8_t **y;
  uint8_t **cb;
  uint8_t **cr;
};


struct imageycbcr16
{
  uint16_t largeur;
  uint16_t hauteur;
  bool couleur;
  int16_t **y;
  int16_t **cb;
  int16_t **cr;
};

#endif
