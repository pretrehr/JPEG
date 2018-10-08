#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <qtables.h>



/* Fonction prenant en entrée un vecteur de 64 représentant un bloc 8x8
 * et le divisant par la table de quantification de Y provenant de qtables.h
 */
int16_t *quantif_y(int16_t *zigzag){
    int16_t *tab = malloc(64*sizeof(int16_t));

    for(int i = 0; i < 64; i++){
        tab[i] = zigzag[i]/compressed_Y_table[i];
    }
    free(zigzag);
    return tab;



}


/* Fonction prenant en entrée un vecteur de 64 représentant un bloc 8x8
 * et le divisant par la table de quantification de Cb et Cr provenant de qtables.h
 */
int16_t *quantif_c(int16_t *zigzag){
    int16_t *tab = malloc(64*sizeof(int16_t));

    for(int i = 0; i < 64; i++){
        tab[i] = zigzag[i]/compressed_CbCr_table[i];
    }
    free(zigzag);
    return tab;


}

#ifdef TEST_QUANTIF
int main(){

    float tab[64] = {1,7.8,9.4,6.2,12.,1.8, .5,15.6,3.5,7.8,9.4,6.2,12.,1.8, .5,15.6, 3.5,7.8,9.4,6.2,12.,1.8, .5,15.6, 3.5,7.8,9.4,6.2,12.,1.8, .5,15.6, 3.5,7.8,9.4,6.2,12.,1.8, .5,15.6, 3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,3.5,7.8,9.4,6.2,12.,1.8, .5,15.6, 3.5,7.8,9.4,6.2,12.,1.8, .5,15.6};

    uint8_t *quantif1 = quantif(tab, 0);
    for(int i = 0; i < 64; i++){
        printf("%i ", quantif1[i]);
    }
}
#endif
