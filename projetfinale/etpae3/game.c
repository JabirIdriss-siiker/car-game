#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "variable.h"
#include "fonction.h"


//game mode 1 afficher le temps de 3tours
int game1(FILE* FichierCircuit)
{
    //initialisation du SDL2
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    //DECLARATION DES VARIABLES

    char ligne[Taille_Max];
    char* Val = (char*)malloc(sizeof(char) * 30);
    Voiture car;
    int nbzones;
    int continuer = 1;
    int depart[2], arrivee[2];
    char* background, * voitureR;
    background = malloc(sizeof(char) * Taille_Max);
    voitureR = malloc(sizeof(char) * Taille_Max);
    int* taille;
    double angle;
    int cpt;
    int checkpoint = 0;
    int toure = 0;
    int tempsactuel = 0;
    char temps[20] = "";
    char tempsfinal[60] = ""  ;

    //variable de SDL
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* circuit = NULL, *Fond = NULL;
    SDL_Surface* ecran = NULL, * texte = NULL,*tempsfinalsurface = NULL;
    SDL_Event Evenement;
    TTF_Font* font = NULL;
    SDL_Color vert = { 0,255,0 };
    SDL_Rect positiontimer;
    SDL_Rect positiontempsfinal;




    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Impossible de charger la bibliothèque\n");
        return 0;
    }
    //Lecture du Fichier du Circuit

    //taille
    fgets(ligne, Taille_Max, FichierCircuit);
    taille = TailleCanvas(ligne);


    //fond
    fgets(ligne, Taille_Max, FichierCircuit);
    Val = strtok(ligne, " \r\n");
    Val = strtok(NULL, "\r\n");
    strcpy(background, Val);


    //nombre de zones
    fgets(ligne, Taille_Max, FichierCircuit);
    nbzones = nbzone(ligne);
    zone tab[nbzones];

    //voiture

    fgets(ligne, Taille_Max, FichierCircuit);
    Val = strtok(ligne, " \r\n");
    Val = strtok(NULL, " \r\n");
    strcpy(voitureR, Val);


    //zone, depart, arrivee

    fgets(ligne, Taille_Max, FichierCircuit);
    for (int i = 0; i < nbzones; i++) {
        Val = strtok(ligne, " ");
        if (strcmp(Val, "ZONE") == 0) {
            Val = strtok(NULL, " \r\n");
            tab[i].type = atoi(Val);
            Val = strtok(NULL, " \r\n");
            strcpy(tab[i].path, Val);
        }
        if (strcmp(Val, "DEPART") == 0) {
            Val = strtok(NULL, " \r\n");
            depart[0] = atoi(Val);
            Val = strtok(NULL, " \r\n");
            depart[1] = atoi(Val);
        }
        if (strcmp(Val, "ARRIVEE") == 0) {
            Val = strtok(NULL, " \r\n");
            arrivee[0] = atoi(Val);
            Val = strtok(NULL, " \r\n");
            arrivee[1] = atoi(Val);
        }
        fgets(ligne, Taille_Max, FichierCircuit);
    }

    Val = strtok(ligne, " \r\n");
    Val = strtok(NULL, " \r\n");
    angle = atof(Val);
    fgets(ligne, Taille_Max, FichierCircuit);

    //CREATION DE LA WINDOW RENDERER etc...

    window = SDL_CreateWindow("RACING CAR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, taille[0] * 100, taille[1] * 100, 0);
    if (window == NULL) {
        SDL_Log("Erreur lors de la création de la fenêtre %s\n", SDL_GetError());
        return -1;
    }

    ecran = SDL_GetWindowSurface(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    if (renderer == NULL) {
        destroy(window, NULL, NULL, NULL, NULL);
        SDL_Log("Erreur lors de la création du renderer %s\n", SDL_GetError());
        return -1;
    }

    Fond = IMG_LoadTexture(renderer, background);
    if (Fond == NULL) {
        destroy(window, renderer, NULL, NULL, NULL);
        SDL_Log("Erreur lors du chargement des textures %s\n", SDL_GetError());
    }
    font = TTF_OpenFont("OpenSans-Bold.ttf", 25);
    if (font == NULL)
    {
      destroy(window, renderer, NULL, NULL, NULL);
      SDL_Log("Erreur lors du chargement des textures %s\n", SDL_GetError());
    }
    SDL_RenderCopy(renderer, Fond, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Rect pos_c;
    pos_c.x = 0;
    pos_c.y = 0;
    pos_c.w = 800 / taille[0];
    pos_c.h = 600 / taille[1];

    SDL_Rect pos_v;
    pos_v.x = depart[0] * (800 / taille[0] + 37);
    pos_v.y = depart[1] * (600 / taille[1] + 14);
    pos_v.w = 25;
    pos_v.h = 45;

    //chargement du circuit

    int carte[taille[1]][taille[0]];
    for (int i = 0; i < taille[1]/*6*/; i++) {
        Val = strtok(ligne, " \n\r");
        for (int j = 0; j < taille[0]/*8*/; j++) {
            cpt = atoi(Val);
            carte[i][j] = cpt;
            circuit = IMG_LoadTexture(renderer, tab[cpt].path);
            SDL_RenderCopy(renderer, circuit, NULL, &pos_c);
            Val = strtok(NULL, " \n\r");
            pos_c.x = pos_c.x + pos_c.w;
        }
        pos_c.x = 0;
        pos_c.y = pos_c.y + pos_c.h;
        fgets(ligne, Taille_Max, FichierCircuit);

    }

    // chargement de la voiture

    car.texture = IMG_LoadTexture(renderer, voitureR);
    if (car.texture == NULL) {
        destroy(window, renderer, Fond, NULL, NULL);
        SDL_Log("Erreur lors du chargement des textures de la voiture %s\n", SDL_GetError());
    }
    SDL_RenderCopyEx(renderer, car.texture, NULL, &pos_v, angle - 90, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    car.vitesse = 0;
    car.inc_v = 0;
    car.inc_angle = 0;

    //boucle du jeu gameplay etc...

    while (continuer) {
        tempsactuel = SDL_GetTicks();
        texte = TTF_RenderText_Solid(font, temps, vert);
        tempsfinalsurface = TTF_RenderText_Solid(font, tempsfinal, vert);
        sprintf(temps, "Chrono: %d", tempsactuel / 1000);
        SDL_PollEvent(&Evenement);

        if (Evenement.type == SDL_QUIT) {
          continuer = 0;
            break;
        }
        //gameplay

        else if(Evenement.type == SDL_KEYDOWN) {

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_UP) {
            if (car.inc_v < 4) {
              car.inc_v++;
            }
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            if (car.inc_v > -3) {
              car.inc_v--;
            }
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_RIGHT && car.inc_angle < 5) {
            car.inc_angle = car.inc_angle+2;
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_LEFT && car.inc_angle > -5) {
            car.inc_angle = car.inc_angle-2;
          }
        }
        else if (Evenement.type == SDL_KEYUP){


          if (Evenement.key.keysym.scancode == SDL_SCANCODE_RIGHT){
            car.inc_angle=0;
          }
          if (Evenement.key.keysym.scancode == SDL_SCANCODE_LEFT){
            car.inc_angle=0;
          }
          if (Evenement.key.keysym.scancode == SDL_SCANCODE_UP){
            car.inc_v=0;
          }
          if (Evenement.key.keysym.scancode == SDL_SCANCODE_DOWN){
            car.inc_v=0;
          }
        }

        if (car.vitesse!=0) {
          angle=angle+car.inc_angle;
          if (angle > 360) {
            angle -= 360;
          }
          else if(angle < -360){
            angle += 360;
          }
        }
        if (car.vitesse < 4 && car.inc_v > 0) {
          car.vitesse = car.vitesse + car.inc_v;
        }
        if (car.vitesse > -3 && car.inc_v < 0) {
          car.vitesse = car.vitesse + car.inc_v;
        }
        if (car.vitesse > 0) {
          car.vitesse--;
        }
        if (car.vitesse < 0) {
          car.vitesse++;
        }
        car.inc_x = car.vitesse * cos(angle/180*M_PI);

        car.inc_y = car.vitesse * sin(angle/180*M_PI);

        //collision avec la fin de l'ecran
        if (carte[pos_v.y/(800/taille[0]) ] [ pos_v.x/(600/taille[1])] == 0 ) {
          car.inc_y=car.inc_y/2;
          car.inc_x=car.inc_x/2;
        }

        //calcul de nombre de tours
        if (carte[pos_v.y / (800 / taille[0])][pos_v.x / (600 / taille[1])] == carte[depart[1]][depart[0]]) {
            if (checkpoint == 1) {
                toure++;
                if (toure == 3) {

                    sprintf(tempsfinal, "Vous avez fait 3 toures en %d secondes", tempsactuel/1000);
                    car.vitesse = 0 ;
                    car.inc_v = 0 ;
                    car.inc_angle = 0 ;
                }

                checkpoint = 0;
           }

        }
        if (carte[pos_v.y / (800 / taille[0])][pos_v.x / (600 / taille[1])] == carte[arrivee[1]][arrivee[0]]) {
            checkpoint = 1;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Fond, NULL, NULL);
        //redessiner le circuit et la voituure a chaque tours de boucle
        for (int i = 0; i < taille[1]; i++) {
            for (int j = 0; j < taille[0]; j++) {
                circuit = IMG_LoadTexture(renderer, tab[carte[i][j]].path);
                SDL_RenderCopy(renderer, circuit, NULL, &pos_c);
                pos_c.x = pos_c.x + pos_c.w;
            }
            pos_c.x = 0;
            pos_c.y = pos_c.y + pos_c.h;
        }
        pos_c.y = 0;
        pos_v.y = pos_v.y - car.inc_y;
        pos_v.x = pos_v.x - car.inc_x;
        if (pos_v.y > taille[1] * 100 - 20) {
            pos_v.y = taille[1] * 100 - 20;
        }
        if (pos_v.y < 0) {
            pos_v.y = 0;
        }
        if (pos_v.x > taille[0] * 100 - 20) {
            pos_v.x = taille[0] * 100 - 20;
        }
        if (pos_v.x < 0) {
            pos_v.x = 0;
        }
        positiontimer.x = 10;
        positiontimer.y = 20;
        positiontempsfinal.x = 250;
        positiontempsfinal.y = 20;

        SDL_BlitSurface(texte, NULL, ecran, &positiontimer);
        SDL_BlitSurface(tempsfinalsurface, NULL, ecran, &positiontempsfinal);
        SDL_RenderCopyEx(renderer, car.texture, NULL, &pos_v, angle - 90, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);

        SDL_Delay(10); // Permet de soulager le CPU pour éviter qu'il tourene à 100%
    }
    free(background);
    free(voitureR);
    fclose(FichierCircuit);
    destroy(window, renderer, Fond, car.texture, circuit);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_FreeSurface(texte);
    SDL_Quit();
    return 0;
}

//game mode 2 contre la montre 30 secondes
//meme code que game1 sauf pour pour la condition de gain
int game2(FILE* FichierCircuit)
{

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    char ligne[Taille_Max];
    char* Val = (char*)malloc(sizeof(char) * 30);
    Voiture car;
    int nbzones = 0;
    int cpt = 0;
    int continuer = 1;
    int depart[2], arrivee[2];
    char* background, *voitureR;
    background = malloc(sizeof(char) * Taille_Max);
    voitureR = malloc(sizeof(char) * Taille_Max);
    int* taille;
    double angle;
    int checkpoint = 0;
    int toure = 0;
    int tempsactuel = 0;
    char temps[20] = "";
    char tempsfinal[60] = "";
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* circuit = NULL, *Fond = NULL;
    SDL_Surface* ecran = NULL, * texte = NULL,*tempsfinalsurface = NULL;

    SDL_Event Evenement;
    TTF_Font* font = NULL;
    SDL_Color vert = { 0,255,0 };
    SDL_Rect positiontimer;
    SDL_Rect positiontempsfinal;



    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Impossible de charger la bibliothèque\n");
        return 0;
    }

    fgets(ligne, Taille_Max, FichierCircuit);
    taille = TailleCanvas(ligne);

    fgets(ligne, Taille_Max, FichierCircuit);
    Val = strtok(ligne, " \r\n");
    Val = strtok(NULL, " \r\n");
    strcpy(background, Val);

    fgets(ligne, Taille_Max, FichierCircuit);
    nbzones = nbzone(ligne);
    zone tab[nbzones];

    fgets(ligne, Taille_Max, FichierCircuit);
    Val = strtok(ligne, " \r\n");
    Val = strtok(NULL, " \r\n");
    strcpy(voitureR, Val);

    fgets(ligne, Taille_Max, FichierCircuit);
    for (int i = 0; i < nbzones; i++) {
        Val = strtok(ligne, " ");
        if (strcmp(Val, "ZONE") == 0) {
            Val = strtok(NULL, " \r\n");
            tab[i].type = atoi(Val);
            Val = strtok(NULL, " \r\n");
            strcpy(tab[i].path, Val);
        }
        if (strcmp(Val, "DEPART") == 0) {
            Val = strtok(NULL, " \r\n");
            depart[0] = atoi(Val);
            Val = strtok(NULL, " \r\n");
            depart[1] = atoi(Val);
        }
        if (strcmp(Val, "ARRIVEE") == 0) {
            Val = strtok(NULL, " \r\n");
            arrivee[0] = atoi(Val);
            Val = strtok(NULL, " \r\n");
            arrivee[1] = atoi(Val);
        }
        fgets(ligne, Taille_Max, FichierCircuit);
    }

    Val = strtok(ligne, " \r\n");
    Val = strtok(NULL, " \r\n");
    angle = atof(Val);
    fgets(ligne, Taille_Max, FichierCircuit);


    window = SDL_CreateWindow("RACING CAR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, taille[0] * 100, taille[1] * 100, 0);
    if (window == NULL) {
        SDL_Log("Erreur lors de la création de la fenêtre %s\n", SDL_GetError());
        return -1;
    }
    ecran = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        destroy(window, NULL, NULL, NULL, NULL);
        SDL_Log("Erreur lors de la création du renderer %s\n", SDL_GetError());
        return -1;
    }

    Fond = IMG_LoadTexture(renderer, background);
    if (Fond == NULL) {
        destroy(window, renderer, NULL, NULL, NULL);
        SDL_Log("Erreur lors du chargement des textures %s\n", SDL_GetError());
    }
    font = TTF_OpenFont("angelina.TTF", 35);
    if (font == NULL)
    {
      destroy(window, renderer, NULL, NULL, NULL);
      SDL_Log("Erreur lors du chargement des textures %s\n", SDL_GetError());
    }
    SDL_RenderCopy(renderer, Fond, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Rect pos_c;
    pos_c.x = 0;
    pos_c.y = 0;
    pos_c.w = 800 / taille[0];
    pos_c.h = 600 / taille[1];

    SDL_Rect pos_v;
    pos_v.x = depart[0] * (800 / taille[0] + 37);
    pos_v.y = depart[1] * (600 / taille[1] + 14);
    pos_v.w = 25;
    pos_v.h = 45;


    int carte[taille[1]][taille[0]];
    for (int i = 0; i < taille[1]/*6*/; i++) {
        Val = strtok(ligne, " \n\r");
        for (int j = 0; j < taille[0]/*8*/; j++) {
            cpt = atoi(Val);
            carte[i][j] = cpt;
            circuit = IMG_LoadTexture(renderer, tab[cpt].path);
            SDL_RenderCopy(renderer, circuit, NULL, &pos_c);
            Val = strtok(NULL, " \n\r");
            pos_c.x = pos_c.x + pos_c.w;
        }
        pos_c.x = 0;
        pos_c.y = pos_c.y + pos_c.h;
        fgets(ligne, Taille_Max, FichierCircuit);

    }


    car.texture = IMG_LoadTexture(renderer, voitureR);
    if (car.texture == NULL) {
        destroy(window, renderer, Fond, NULL, NULL);
        SDL_Log("Erreur lors du chargement des textures de la voiture %s\n", SDL_GetError());
    }
    SDL_RenderCopyEx(renderer, car.texture, NULL, &pos_v, angle - 90, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    car.vitesse = 0;
    car.inc_v = 0;
    car.inc_angle = 0;

    while (continuer) {
        tempsactuel = SDL_GetTicks();
        texte = TTF_RenderText_Solid(font, temps, vert);
        tempsfinalsurface = TTF_RenderText_Solid(font, tempsfinal, vert);
        sprintf(temps, "vous avez 30 sec pour finir : %d", tempsactuel / 1000);
        SDL_PollEvent(&Evenement);

        if (Evenement.type == SDL_QUIT) {
          continuer = 0;
            break;
        }
        else if(Evenement.type == SDL_KEYDOWN) {

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_UP) {
            if (car.inc_v < 4) {
              car.inc_v++;
            }
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            if (car.inc_v > -3) {
              car.inc_v--;
            }
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_RIGHT && car.inc_angle < 5) {
            car.inc_angle = car.inc_angle+2;
          }

          if (Evenement.key.keysym.scancode == SDL_SCANCODE_LEFT && car.inc_angle > -5) {
            car.inc_angle = car.inc_angle-2;
          }
        }
        else if (Evenement.type == SDL_KEYUP){


          if (Evenement.key.keysym.scancode == SDL_SCANCODE_RIGHT){
            car.inc_angle=0;
          }
          if (Evenement.key.keysym.scancode == SDL_SCANCODE_LEFT){
            car.inc_angle=0;
          }
          if (Evenement.key.keysym.scancode == SDL_SCANCODE_UP){
            car.inc_v=0;
          }
          if (Evenement.key.keysym.scancode == SDL_SCANCODE_DOWN){
            car.inc_v=0;
          }
        }

        if (car.vitesse!=0) {
          angle=angle+car.inc_angle;
          if (angle > 360) {
            angle -= 360;
          }
          else if(angle < -360){
            angle += 360;
          }
        }
        if (car.vitesse < 4 && car.inc_v > 0) {
          car.vitesse = car.vitesse + car.inc_v;
        }
        if (car.vitesse > -3 && car.inc_v < 0) {
          car.vitesse = car.vitesse + car.inc_v;
        }
        if (car.vitesse > 0) {
          car.vitesse--;
        }
        if (car.vitesse < 0) {
          car.vitesse++;
        }
        car.inc_x = car.vitesse * cos(angle/180*M_PI);

        car.inc_y = car.vitesse * sin(angle/180*M_PI);
        if (carte[pos_v.y/(800/taille[0]) ] [ pos_v.x/(600/taille[1])] == 0 ) {
          car.inc_y=car.inc_y/2;
          car.inc_x=car.inc_x/2;

        }
        if (carte[pos_v.y / (800 / taille[0])][pos_v.x / (600 / taille[1])] == carte[depart[1]][depart[0]]) {
            if (checkpoint == 1) {
                toure++;
                checkpoint = 0;
           }

        }
        if (carte[pos_v.y / (800 / taille[0])][pos_v.x / (600 / taille[1])] == carte[arrivee[1]][arrivee[0]]) {
            checkpoint = 1;
        }
        // condition de gain si le temps est superieure a 30 secondes ca affiche vous avez perdu

        if ((tempsactuel / 1000) > 3)
        {

            sprintf(tempsfinal, " Vous avez perdu ", tempsactuel);

            car.inc_angle = 0;
            car.vitesse = 0;
            car.inc_v = 0;

        }
        //sinon si vous faites 3 tours en moins de 30 secondes vous gagnez
        else if (tempsactuel/1000 < 30 && toure == 3) {

            sprintf(tempsfinal, "Vous avez gagne", tempsactuel);

            car.inc_angle = 0;
            car.vitesse = 0;
            car.inc_v = 0;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, Fond, NULL, NULL);
        for (int i = 0; i < taille[1]; i++) {
            for (int j = 0; j < taille[0]; j++) {
                circuit = IMG_LoadTexture(renderer, tab[carte[i][j]].path);
                SDL_RenderCopy(renderer, circuit, NULL, &pos_c);
                pos_c.x = pos_c.x + pos_c.w;
            }
            pos_c.x = 0;
            pos_c.y = pos_c.y + pos_c.h;
        }
        pos_c.y = 0;
        pos_v.y = pos_v.y - car.inc_y;
        pos_v.x = pos_v.x - car.inc_x;
        if (pos_v.y > taille[1] * 100 - 20) {
            pos_v.y = taille[1] * 100 - 20;
        }
        if (pos_v.y < 0) {
            pos_v.y = 0;
        }
        if (pos_v.x > taille[0] * 100 - 20) {
            pos_v.x = taille[0] * 100 - 20;
        }
        if (pos_v.x < 0) {
            pos_v.x = 0;
        }
        positiontimer.x = 10;
        positiontimer.y = 20;
        positiontempsfinal.x = taille[0] / 2 *100 - 100;
        positiontempsfinal.y = taille[1] / 2 *100 - 50;

        SDL_BlitSurface(texte, NULL, ecran, &positiontimer);
        SDL_BlitSurface(tempsfinalsurface, NULL, ecran, &positiontempsfinal);

        SDL_RenderCopyEx(renderer, car.texture, NULL, &pos_v, angle - 90, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);

        SDL_Delay(10); // Permet de soulager le CPU pour éviter qu'il tourene à 100%
    }
    free(background);
    free(voitureR);
    fclose(FichierCircuit);
    destroy(window, renderer, Fond, car.texture, circuit);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_FreeSurface(texte);
    SDL_Quit();
    return 0;
}
