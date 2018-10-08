#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <huffman.h>
#include <jpeg_writer.h>

struct jpeg_desc;

/* Fonction prenant en entrée tous les paramètres à mettre dans le Header du jpeg
 * et appelant toutes les fonctions de jpeg_writer pour mettre à jour tous les 
 * paramètres de la structure jpeg_desc.
 * Entrée : Tous les paramètres nécessaires à l'écriture du Header
 * Sortie : La structure jpeg_desc correspondante
 */

struct jpeg_desc *entete(char *ppm_filename, 
                        char *jpeg_filename,
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
                        uint8_t *qtable_c){
                            

    struct jpeg_desc *jpeg = jpeg_desc_create();
    
    jpeg_desc_set_ppm_filename(jpeg, ppm_filename);

    jpeg_desc_set_jpeg_filename(jpeg, jpeg_filename);

    jpeg_desc_set_image_height(jpeg, image_height);

    jpeg_desc_set_image_width(jpeg, image_width);

    jpeg_desc_set_nb_components(jpeg, nb_components);

    jpeg_desc_set_sampling_factor(jpeg, Y, H, sampling_factor[0]);
    jpeg_desc_set_sampling_factor(jpeg, Y, V, sampling_factor[1]);

    jpeg_desc_set_huffman_table(jpeg, AC, Y, table_y_ac) ;
    jpeg_desc_set_huffman_table(jpeg, DC, Y, table_y_dc) ;
    
    jpeg_desc_set_quantization_table(jpeg, Y, qtable_y);
    
    if(nb_components == 3){
        jpeg_desc_set_huffman_table(jpeg, AC, Cb, table_cb_ac); 
        jpeg_desc_set_huffman_table(jpeg, AC, Cr, table_cr_ac) ;
        jpeg_desc_set_huffman_table(jpeg, DC, Cb, table_cb_dc); 
        jpeg_desc_set_huffman_table(jpeg, DC, Cr, table_cr_dc) ;

        jpeg_desc_set_quantization_table(jpeg, Cb, qtable_c);
        jpeg_desc_set_quantization_table(jpeg, Cr, qtable_c);
    
        jpeg_desc_set_sampling_factor(jpeg, Cb, H, sampling_factor[2]);
        jpeg_desc_set_sampling_factor(jpeg, Cb, V, sampling_factor[3]);

        jpeg_desc_set_sampling_factor(jpeg, Cr, H, sampling_factor[4]);
        jpeg_desc_set_sampling_factor(jpeg, Cr, V, sampling_factor[5]);

    }

    return jpeg;
}

