#ifndef LECTURE_ECRITURE_H
#define LECTURE_ECRITURE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <bitstream.h>
#include <huffman.h>
#include <jpeg_writer.h>
#include <structures.h>


extern void char_to_hv(uint8_t *h[], uint8_t *v[], char *chaine);

extern void get_option(int argc,char **argv, bool *verbose,char **nom_jpeg,
                char **nom_ppm, uint8_t *h[], uint8_t *v[]);

extern char *ReadLine(FILE *fichier);

extern struct image lecture_image(char *nom_fichier, bool verbose, char* nom_jpeg, uint8_t* h, uint8_t* v);

struct jpeg_desc;

extern struct jpeg_desc *entete(const char *ppm_filename,
                            const char *jpeg_filename,
                            uint32_t image_height,
                            uint32_t image_width,
                            uint8_t nb_components,
                            uint8_t sampling_factor[nb_components*2],
                            struct huff_table *table_y_ac,
                            struct huff_table *table_y_dc,
                            struct huff_table *table_cb_ac,
                            struct huff_table *table_cb_dc,
                            struct huff_table *table_cr_ac,
                            struct huff_table *table_cr_dc,
                            uint8_t *qtable_y,
                            uint8_t *qtable_c
                            );

extern void encode(struct bitstream *stream, int16_t quantif[64], int16_t *dc, struct huff_table *table_dc, struct huff_table *table_ac, uint8_t *nbits);


#endif
