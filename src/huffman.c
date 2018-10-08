#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* Structure non opaque contenant:
 * - Un vecteur à 16 entrés contenant le nombre de symbole par longueur de code
 * - Les symboles correspondant ordonnés par probabilité d'apparition
 * - Le nombre total de symboles
 * - Les codes des symboles ordonnés par la valeur des symboles correspondant
 * - La taille des symboles ordonnées par la valeur des symboles correspondant
 */
struct huff_table
{
    uint8_t *nb_symb_per_lengths;
    uint8_t *symbols;
    uint8_t nb_symbols;
    uint32_t *ehufcode; //Codes des symboles ordonnés par la valeur de ceux-ci
    uint8_t *ehufsize; //Longueur des codes des symboles ordonnées par la valeur de ceux-ci

};


/* Fonction créant une table de Huffman à partir des données de htables
 * Entrée : - Un vecteur à 16 entrés contenant le nombre de symbole par longueur de code
 *          - Les symboles correspondant ordonnés par probabilité d'apparition
 *          - Le nombre total de symboles
 * Sortie : - La structure table de Huffman correspondant
 */
struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths,
                                       uint8_t *symbols,
                                       uint8_t nb_symbols){
    struct huff_table *table = malloc(sizeof(struct huff_table));

    
    // Création d'un tableau contenant les taille des codes 
    // des symboles de symbols
    
    uint8_t *huffsize = malloc((1+nb_symbols)*sizeof(uint8_t));
    uint8_t k1 = 0;
    for (int i = 0; i <= 15; i++){
        for (int j = 1; j <= nb_symb_per_lengths[i]; j++){
            huffsize[k1] = i+1;
            k1++;
        }
    }
    huffsize[k1] = 0;

    // Création d'un tableau contenant les codes 
    // des symboles de symbols
    uint32_t *huffcode = malloc((nb_symbols+1)*sizeof(uint32_t));
    uint8_t k = 1, si = huffsize[0];
    uint32_t code = 1;
    huffcode[0] = 0;
    while ( k <= 255){
        while (huffsize[k] == si){
            huffcode[k] = code;
            code++;
            k++;
        }
        
        if (huffsize[k] == 0) break;

        do{
            code <<= 1;
            si++; 
        
        
        }while (huffsize[k] != si);
    }
    table->nb_symb_per_lengths = nb_symb_per_lengths;
    table->symbols = symbols;
    table->nb_symbols = nb_symbols;
    
    // Création de tableaux contenant les taille des codes 
    // et d'un contenant les codes des symboles indicés par la valeur des symboles
    k = 0;
    uint32_t *ehufcode = malloc(255*sizeof(uint32_t));
    uint8_t *ehufsize = malloc(255*sizeof(uint8_t));
    uint8_t i;
    do{
        i = symbols[k];
        ehufcode[i] = huffcode[k];
        ehufsize[i] = huffsize[k];
        k++;

    
    }while ( k < k1);
    free(huffsize);

    free(huffcode); 
    table->ehufcode = ehufcode;
    table->ehufsize = ehufsize;
    return table;
}





/* Fonction permettant de récupérer le vecteur de symboles d'une
 * structure table de Huffman
 */
uint8_t *huffman_table_get_symbols(struct huff_table *ht){
    return ht->symbols;
}


/* Fonction permettant de récupérer le vecteur de nombre de symbole par longueur 
 * de code d'une structure table de Huffman
 */
uint8_t *huffman_table_get_length_vector(struct huff_table *ht){
    return ht->nb_symb_per_lengths;
}


/* Fonction permettant de récupérer le code et la taille de ce code à partir 
 * d'un symbole dans une table de Huffman
 */
uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nbits){
    *nbits = ht->ehufsize[value];
    return ht->ehufcode[value];
}

/* Fonction libérant la place mémoire occupée par une table Huffman */
void huffman_table_destroy(struct huff_table *ht){
    free(ht->ehufsize);
    free(ht->ehufcode);
    free(ht);
}
