#ifndef FONCTION_BLOC_H
#define FONCTION_BLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <structures.h>


struct imageycbcr16 f_image8_16(struct imageycbcr8 img, int16_t* (*f)(uint8_t[64]), uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);

void imageycbcr8_destroy(struct imageycbcr8);

void imageycbcr16_destroy(struct imageycbcr16);

void image_destroy(struct image img);

void imageycbcr16_destroy_complet(struct imageycbcr16 img, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);

struct imageycbcr16 f_image16_16(struct imageycbcr16 img, int16_t* (*f)(int16_t[64]), uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);

struct imageycbcr16 f_g_image16_16(struct imageycbcr16 img, int16_t* (*f)(int16_t[64]), int16_t* (*g)(int16_t[64]), uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);

#endif
