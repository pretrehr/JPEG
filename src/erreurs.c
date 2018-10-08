#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<assert.h>


void erreur(const char* message) {
  fprintf(stderr, "%s\n", message);
  exit(EXIT_FAILURE);
}
