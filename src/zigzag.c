#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* Fonction prenant en entrée un vecteur de 64 représenant un bloc 8x8
 * et le retournant dans l'ordre zigzag décrit par la norme JPEG
 */
int16_t *zigzag(int16_t dct[64]){
    int16_t *tab = malloc(64*sizeof(int16_t));
    int n = 0;
    int pas = 0;
    int i;
    int j;
    while(pas < 7){
        i = pas;
        j = pas - i;
        while( i >= 0 ){
            tab[n] = dct[i*8+j];
            n += 1;
            i -= 1;
            j = pas - i;
        }
        pas += 1;
        i += 1;
        while( j >= 0 ){
            tab[n] = dct[i*8+j];
            n += 1;
            j -= 1;
            i = pas - j;

        }
        pas += 1;
        i += 1;
    }
    while(pas < 15){
        i = 7;
        j = pas - i;
        while( j < 7 ){
            tab[n] = dct[i*8+j];
            n += 1;
            j += 1;
            i = pas - j;
        }
        pas += 1;
        while( i <= 7 ){
            tab[n] = dct[i*8+j];
            n += 1;
            i += 1;
            j = pas - i;
        }
        pas += 1;

    }
    free(dct);
    return tab;
}

#ifdef TEST_ZIGZAG
int main(){
    float tab[64] = { 1  ,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6,
                      3.5,7.8,9.4,6.2,12.,1.8, .5,15.6};
    float *zigzag1 = zigzag(tab) ;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){

            printf("%f ", zigzag1[i*8+j]);
        }
        printf("\n");

    }
  }
#endif
