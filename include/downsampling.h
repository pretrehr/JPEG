#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <structures.h>



struct imageycbcr8 decoupe_mcu(struct image img, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);

struct imageycbcr8 decoupe_suiv(struct imageycbcr8 img, uint8_t h1, uint8_t v1, uint8_t h2, uint8_t v2, uint8_t h3, uint8_t v3);
