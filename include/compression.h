#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <structures.h>


extern struct imageycbcr8 ycbcr(struct image img);

extern int16_t *dct(uint8_t *tab);

extern int16_t *zigzag(int16_t *dct);

extern int16_t *quantif_y(int16_t *zigzag);

extern int16_t *quantif_c(int16_t *zigzag);

extern float *produit_mat(float* a, float* b);


extern float *transposee(float* a);

extern int16_t arrondi16(float x);

extern int16_t *dct_rapide(uint8_t* a);

extern struct image redim_gris(struct image img);

extern struct image redim_couleur(struct image img, uint8_t h1, uint8_t v1);

#endif /* TRANSFO_H */
