#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <erreurs.h>
/********************************************************************

Entrée : Deux tableau pour stocker les valeurs h et v, la chaine

Do : traduit h1xv1,h2xv2,h3xv3 en h = [h1, h2, h3] et v = [v1, v2, v3]
Lève une exeption si la chaine ne suis pas le bon format

*******************************************************************/


void char_to_hv(uint8_t *h[], uint8_t *v[], char *chaine)

{
  int i = 0;
  int posh = 0;
  int posv = 0;
  int taille_nb = 0;
  while (chaine[i] != '\0')
  {
    if(taille_nb >= 2)//si on a un h1 à deux chiffre
    {
      printf("%i\n", i);
      printf("%s\n", chaine);
      free(*h);
      free(*v);
      erreur("Arguments trop grands : l'un des arguments est >= 10");
    }
    else if(chaine[i] == 'x')
    {
      if(i%4 != 1)//le x n'est pas à la bonne place
      {
        free(*h);
        free(*v);
        erreur("les x sont mal places");
      }
      taille_nb = 0;
    }
    else if (chaine[i] == ',')
    {
      if(i%4 !=3)
      {
        free(*h);
        free(*v);
        erreur("les , sont mal placees");
      }
      taille_nb = 0;
    }
    else if (isdigit(chaine[i]))
    {
      if(i%4 ==0)
      {
        h[0][posh] = atoi(&chaine[i]);
        taille_nb++;
        posh++;
      }
      else
      {
        v[0][posv] = atoi(&chaine[i]);
        taille_nb++;
        posv++;
      }
    }
    else
    {
      free(*h);
      free(*v);
      erreur("Ne respecte pas le format impose, voir --help pour plus d'aide");
    }
    i++;
  }
    if((h[0][0])*(v[0][0])+(h[0][1])*(v[0][1])+(h[0][2])*(h[0][2])>10)
    {
      free(*h);
      free(*v);
      erreur("Arguments trop grands : somme des produits > 10");
    }
    if ((h[0][0]%h[0][1])!=0 || (h[0][0]%h[0][2])!=0 || (v[0][0]%v[0][1])!=0 || (v[0][0]%v[0][2])!=0)
    {
      free(*h);
      free(*v);
      erreur("Une des dimensions de sous-echantillonage n'est pas divisible par h1 ou v1");
    }
    if (h[0][0]>4 || v[0][0]>4)
    {
      free(*h);
      free(*v);
      erreur("Un des dimensions de MCU est > 4");
    }
}



void get_option(int argc,char **argv, bool *verbose,char **nom_jpeg,
                char **nom_ppm, uint8_t *h[], uint8_t *v[]) {
  bool no_arg = 0;
  bool yes_arg = 1;
  bool nvxnom = 0;
  int long_index = 0;
  int opt;
  char jpg[5] = ".jpeg";
  struct option long_options[] = {
    {"help", no_arg, 0, 'h'},
    {"outfile", yes_arg, 0, 'o'},
    {"sample", yes_arg, 0, 's'},
    {"verbose", no_arg, 0, 'v' },
    {"dest", yes_arg, 0, 'd'},
    {0, 0, 0, 0}
  };
  while ((opt = getopt_long(argc, argv, "ho:s:v",
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 'h' :
              {
                printf("                == Voici les options ==\n");
                printf("\n");
                printf("--help                     : affiche la page d'aide\n");
                printf("-- verbose                 : option verbeuse\n");
                printf("--outfile=sortie.jpg       : pour redéfinir le nom du fichier de sortie\n");
                printf("--sample=h1xv1,h2xv2,h3xv3 : pour définir les facteurs d’échantillonnage h × v des trois composantes de couleur\n");
                printf("                   == Sans options ==\n");
                printf("\n");
                printf("Le fichier de sortie s'appellera nom_fichier_entree.jpeg et se situera dans le repetoire courant\n");
              }
                 break;
             case 'o' :
             {
                *nom_jpeg = calloc(strlen(optarg), sizeof(char *));
                strcpy(*nom_jpeg, optarg);
                nvxnom = 1;
             }
                 break;
             case 's' :
             {
                char_to_hv(h, v, optarg);
             }
                 break;
             case 'v' : *verbose = 1;
                 break;
             default: printf("Nom d'option invalide !\n");
                 free(*h);
                 free(*v);
                 free(*nom_jpeg);
                 exit(EXIT_FAILURE);
        }
  }
  if (optind < argc) {
    *nom_ppm = argv[optind];
  }
  else
  {
    erreur("Pas de nom de fichier valide");
  }
  if (!nvxnom)
  {
    *nom_jpeg = calloc(strlen(*nom_ppm)+2, sizeof(char));
    int i = 0;
    while(nom_ppm[0][i] != '.')
    {
      nom_jpeg[0][i] = nom_ppm[0][i];
      i++;
      if (nom_ppm[0][i] == '\0')
      {
        free(*h);
        free(*v);
        free(*nom_jpeg);
        erreur("le fichier d'origine n'a pas d'extension !");
      }
    }
    for(int j=0; j<5; j++)
    {
      nom_jpeg[0][i+j] = jpg[j];
    }
  }
if(*verbose)
{
  printf("l'option verbose est active\n");
  printf("le nom du fichier ppm est : %s\n", *nom_ppm);
  printf("le nom du fichier de sortie est : %s\n", *nom_jpeg);
  printf("les dimensions sont : %i x %i , %i x %i, %i x %i\n", h[0][0], v[0][0], h[0][1], v[0][1], h[0][2], v[0][2]);
}
else
{
  printf("L'option verbose n'est pas active\n");
}
}
