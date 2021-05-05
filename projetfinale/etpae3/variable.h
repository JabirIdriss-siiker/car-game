#ifndef RACING_CAR_VAR_H
#define RACING_CAR_VAR_H


#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define Taille_Max 70

typedef struct zone {
    int type;
    char path[Taille_Max];
}zone;

typedef struct {
    char path[Taille_Max];
    double vitesse;
    double inc_v;
    double inc_x;
    double inc_y;
    double inc_angle;
    SDL_Texture *texture;
}Voiture;


#endif
