
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "variable.h"
#include "fonction.h"

int* TailleCanvas(char* ligne) {
    char* CurseurValeur;
    int* taillezonejeu = malloc(sizeof(int) * 2);
    CurseurValeur = strtok(ligne, " \r\n");
    CurseurValeur = strtok(NULL, " \r\n");
    taillezonejeu[0] = atoi(CurseurValeur);
    CurseurValeur = strtok(NULL, " \r\n");
    taillezonejeu[1] = atoi(CurseurValeur);
    return taillezonejeu; //retourne 8 6 dans "TAILLE 8 6"
}

int nbzone(char* ligne) {
    char* CurseurValeur = malloc(sizeof(char) * 512);
    int NombreZones;
    CurseurValeur = strtok(ligne, " \r\n");
    CurseurValeur = strtok(NULL, " \r\n");
    NombreZones = atoi(CurseurValeur);
    return NombreZones; //retourne 16 dans "NBZONES 16"

}


void destroy(SDL_Window* w, SDL_Renderer* r, SDL_Texture* h, SDL_Texture* v, SDL_Texture* c) {
    if (c != NULL) {
        SDL_DestroyTexture(c);
    }
    if (v != NULL) {
        SDL_DestroyTexture(v);
    }
    if (h != NULL) {
        SDL_DestroyTexture(h);
    }
    if (r != NULL) {
        SDL_DestroyRenderer(r);
    }
    if (w != NULL) {
        SDL_DestroyWindow(w);
    }
    SDL_Quit();
}
