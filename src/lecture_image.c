#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include<structures.h>
#include<erreurs.h>
/*******************************************************************************

Entrée : pointeur vers un fichier

Sortie : la 1ere ligne non commentée de ce fichier (la fin de ligne est soit \n soit ' ')
Enlève les commentaires de fin de ligne

*******************************************************************************/
char *ReadLine(FILE *fichier)
{
  int i = 0;
  int taille_ptr = 1;
  char *ptr = calloc(1, sizeof(char *));
  char buffer = '\0';

  while(buffer != ' ' && buffer != '\n')
  {
    buffer = fgetc(fichier);
    if(i+1 == taille_ptr && buffer != ' ' && buffer != '\n')
    {
      taille_ptr *= 2;
      ptr = realloc(ptr, sizeof(char *)*taille_ptr);
    }
    if (buffer == '#')
    {
        char *tampon = NULL;
        size_t longueur = 0;
        getline(&tampon, &longueur, fichier);
        free(tampon);
        buffer = fgetc(fichier);
    }
    ptr[i] = buffer;
    i++;
  }  return ptr;
}
/**********************************************************

Entrée : pointeur vers un fichier, booléen qui indique si
l'option verbose est active

Sortie : une structure image contenant les partie utile de
l'entete (un booléen couleur, largeur et hauteur)


***************************************************/
struct image lecture_image(char *nom_fichier, bool verbose, char* nom_jpeg, uint8_t* h, uint8_t* v)
{
   int buffer;
   char *linebuffer = NULL;
   char *magicNum = calloc(3, sizeof(char *));
   uint16_t largeur = 0;
   uint16_t hauteur = 0;
   uint16_t plage = 0;
   bool coul = 0;
   printf("%s\n", nom_fichier);
   FILE* fichier = fopen(nom_fichier, "r");
   uint8_t *tab = NULL;
   if(fichier == NULL)
   {
     free(magicNum);
     free(nom_jpeg);
     free(h);
     free(v);
     erreur("fichier introuvable");
   }
     for(int i = 0; i < 4; i++)
     {

       linebuffer = ReadLine(fichier);
       if(i==0)
       {
         strcpy(magicNum, linebuffer);
         free(linebuffer);
       }
       if (i==1)
       {
         largeur = atoi(linebuffer);
         free(linebuffer);
       }
       if(i==2)
       {
         hauteur = atoi(linebuffer);
         free(linebuffer);
       }
       if(i==3)
       {
         plage = atoi(linebuffer);
         free(linebuffer);
         if(plage != 255)
         {
           printf("la plage vaut %u\n", plage);
           fclose(fichier);
           free(magicNum);
           erreur("La plage de ce fichier devait valoir 255");
         }
       }
     }
      if(verbose)
      {
        printf("magicNum = %c%c\n", magicNum[0], magicNum[1]);
        printf("largeur = %i\n", largeur);
        printf("hauteur = %i\n", hauteur);
        printf("plage = %u\n", plage);
      }
        buffer = fgetc(fichier);

       if(strcmp(magicNum, "P5\n") == 0)
       {

         uint32_t j = 0;
         tab = malloc(hauteur*largeur*sizeof(uint8_t)+1);
         while (buffer != EOF)
         {
             coul = 0;
             tab[j] = buffer;

           buffer = fgetc(fichier);
           j++;
         }
       }
       else if(strcmp(magicNum, "P6\n") == 0)
       {
         uint32_t j = 0;
         tab = malloc(3*hauteur*largeur*sizeof(uint8_t)+3);
         while (buffer != EOF)
         {
           coul = 1;
           tab[j] = buffer;
           buffer = fgetc(fichier);
           j++;
         }
       }
       else
       {
         fclose(fichier);
         free(magicNum);
         erreur("Fichier non supporte");
       }
       if (verbose){
       fseek(fichier,0, SEEK_END);
       uint32_t taille = ftell(fichier);
       printf("La taille du fichier original est : %i\n", taille);
       fclose(fichier);
       }
   struct image img = {largeur, hauteur, coul, tab};
   free(magicNum);
   return img;
}

#ifdef TEST_IMAGE
int main(void)
{
  lecture_image("invader.pgm", 1);
  return 0;
}
#endif
