#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <htables.h>
#include <huffman.h>
#include <bitstream.h>
#include <jpeg_writer.h>


/* Fonction logarithme base 2
 * Entrée : int16_t
 * Sortie : Partie entière du log2
 */
   

int16_t loga2(int16_t diff1){
    int16_t l = 0;
    while(diff1 > 1){
        diff1 /= 2;
        l++;
    }
    return l;
}


/* Fonction encodant les valeurs DC et AC des blocs 8x8 selon la norme JPEG
 * Entrée : - Le bitstream sur lequel on encode
 *          - Un tableau 64 représentant un bloc 8x8 fréquentiel
 *          - Un pointeur contenant la valeur DC du précédent bloc
 *          - Deux tables de Huffman pour encoder respectivement DC et AC
 *          - Un pointeur nbits utilisé par les fonctions appelées
 *
 * Sortie : - Le bloc entièrement codé dans le bitstream
 *          - La valeur du DC actuel permettant les calculs pour le bloc suivant
 */

void encode(struct bitstream *stream, int16_t quantif[64], int16_t *dc, 
            struct huff_table *table_dc, struct huff_table *table_ac, uint8_t *nbits){


    int16_t diff = quantif[0] - *dc;
    uint8_t mag = 0;
    
    //Codage de DC
    
    *dc = quantif[0];
    
    if (diff == 0) mag = 0;
    else mag = loga2(abs(diff)) + 1;
    
    uint32_t code_mag = huffman_table_get_path(table_dc, mag, nbits);
    
    bitstream_write_nbits(stream, code_mag, *nbits, false);
    
    
    uint32_t code_ind = diff + ((1 << mag) - 1)*(int)(diff < 0);
    
    
    bitstream_write_nbits(stream, code_ind, mag, false);

    //Codage AC

    int i = 1;
    int nb_zero = 0;
    uint32_t code_ac;
    uint8_t code_16_nulles = 0xF0;
    uint8_t rle_ac;
    while(i < 64){ //tant qu'on est dans le bloc
        

    
        while(i < 64 && quantif[i] == 0){  //Tant qu'on lit des zéros on incrémente juste nb_zero
            i++;
            nb_zero++;
        }

        if( i < 64 && quantif[i] != 0){//Si on sort du bloc avec un zero, on va directement à fin_ac 
            while (nb_zero >= 16){
                code_ac = huffman_table_get_path(table_ac, code_16_nulles, nbits);                    
                bitstream_write_nbits(stream, code_ac, *nbits, false);
                nb_zero -= 16;
            }
    
            diff = quantif[i];
            mag = loga2(abs(diff)) + 1;
    

            rle_ac = nb_zero * 16 + mag;
            code_ac = huffman_table_get_path(table_ac, rle_ac, nbits);
            bitstream_write_nbits(stream, code_ac, *nbits, false);
            code_ind = diff + ((1 << mag) - 1)*(int)(diff < 0);
    
            bitstream_write_nbits(stream, code_ind, mag, false);


            nb_zero = 0;
            i++;
        }

    }
    if (nb_zero != 0){ //Marqueur End of Block seulement si la dernière valeur est nulle
        uint8_t fin_ac = 0x00;
        code_ac = huffman_table_get_path(table_ac, fin_ac, nbits);
        bitstream_write_nbits(stream, code_ac, *nbits, false);
        
    }
}

    

    
    









