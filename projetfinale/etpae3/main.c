#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "variable.h"
#include "fonction.h"
#include "game.h"

int main(int argc, char const *argv[]) {
  int mode;
  FILE* FichierCircuit;
  if(argc != 2)
{
 printf("Vous devez entrez 2 arguments \n" );

}
FichierCircuit = fopen(argv[1], "r" );
if(!FichierCircuit)//gerer la mauvaise lecture du fichier
{
 fprintf(stderr, "Erreur : lecture du fichier impossible !") ;
}

  if (FichierCircuit == NULL) {
      printf("Ouverture du fichier impossible\n");
      return 0;
  }
  printf("AVANT DE COMMENCER A JOUER LISEZ LE FICHIER README\n");
  printf("CHOISISSEZ VOTRE MODE DE JEU\n");
  printf("meilleur temps: 1\n");
  printf("contre la montre:2 \n");
  scanf("%d",&mode);
  if(mode == 1)
  {
    game1(FichierCircuit);
  }
  else if(mode == 2)
  {
    game2(FichierCircuit);
  }
  else
  {
    printf("Entrez un mode valable\n");
  }

  return 0;
}
