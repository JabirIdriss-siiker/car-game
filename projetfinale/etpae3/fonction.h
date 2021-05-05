#ifndef RACING_CAR_FCT_H
#define RACING_CAR_FCT_H


#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int* TailleCanvas(char* );
int nbzone(char* );
void destroy(SDL_Window* , SDL_Renderer* , SDL_Texture* , SDL_Texture* , SDL_Texture*);
#endif
