#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define pi 3.14159265358979323846
/*
    Fonction DCT naïve
    Entrée : Tableau de 64 uint8_t représentant un bloc 8x8 spatialement
    Sortie : Tableau de 64 int16_t représentant le bloc 8x8 fréquentiellement
*/

float C(int i){
    if(i == 0){
        return 1/sqrt(2);
    }
    else{
        return 1;
    }
}

int16_t *dct(uint8_t tab[64]){

    int16_t *dct1 = malloc(64*sizeof(int16_t));
    for(uint32_t k = 0; k < 64; k++){
        dct1[k] = (int16_t)(tab[k]) - 128;
    }

    int16_t *dct2 = malloc(64*sizeof(int16_t));
    float res;
    float k;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            res = 0;
            for(int x = 0; x < 8; x++){
                for(int y = 0; y < 8; y++){
                    res += dct1[x*8+y]*cos((2*x + 1)*i*pi/2/8)*cos((2*y + 1)*j*pi/2/8);
                }
            }
            k = 0.25 * C(i) * C(j);
            res *= k;
            dct2[i*8+j] = (int16_t)res;

        }
    }
    free(tab);
    free(dct1);
    return dct2;
}


#ifdef TEST_DCT
int main(){

    uint8_t tab[64] = {0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0};

    int16_t *dct1 = dct(tab);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            printf("%d ", dct1[i*8+j]);
        }
        printf("\n");

    }


}
#endif
