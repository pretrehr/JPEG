#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <matc.h>
#define pi 3.14159265358979323846

/***************************************

Calcule le produit matriciel de deux matrices de float

***************************************/

float* produit_mat(float* a, float* b)
{
  float s=0;
  float *c = (float *)calloc(64,sizeof(float));
  for (int i=0; i<8; i++)
  {
    for (int j=0; j<8; j++)
    {
      for (int k=0; k<8; k++)
      {
        s += a[i*8+k]*b[k*8+j];
      }
      c[i*8+j] = s;
      s = 0;
    }
  }
  return c;
}


/***************************************

Calcule la transposee d'une matrice de float

***************************************/


float* transposee(float* a)
{
  float *b = (float *)calloc(64,sizeof(float));
  for (int i=0; i<8; i++)
  {
    for (int j=0; j<8; j++)
    {
      b[i*8+j] = a[j*8+i];
    }
  }
  return b;
}


/***************************************

Fait un arrondi à l'entier le plus proche

***************************************/

int16_t arrondi16(float x)
{
  uint16_t y = (int16_t)(x+0.5);
  return y;
}

/***************************************

Calcule la Transformee en Cosinus Discrete a l'aide d'une table de cosinus

***************************************/

int16_t *dct_rapide(uint8_t *a)
{
  float *b = (float *)calloc(64, sizeof(float));
  for (int i=0; i<64; i++)
  {
    b[i] = (float)(a[i]-128);
  }

  float *inter = produit_mat(matc, b);

  float *c =  produit_mat(inter, matd);

  int16_t *d = (int16_t *)calloc(64,sizeof(int16_t));
  for (int i = 0; i<64; i++)
  {
    d[i] = arrondi16(c[i]);
  }
  free(c);
  free(inter);
  free(b);
  free(a);
  return d;
}

#ifdef TEST_DCTR
int main()
{
  float *b = (float *)calloc(64,sizeof(float));
  for (int i = 0; i<64; i++)
  {
    b[i] = 128;
    printf("%f", b[i]);
  }
  affiche_tableau(dct_rapide(b));
  return 0;
}
#endif
