#include <SDL2/SDL.h>

int NombreDansChaine(char*, const int);

void Position(char*, char*, const int);

int ChargementLargeurHauteur(FILE*,int*);

int ChargementTuile(FILE*,char[][256], int*);

int ChargementDecor(FILE*, int*, int*, int*);

int ChargementObjetEtPlayer(FILE*,char[][256], int[][2],char*, int*);

int Chargement(int*, int**, char[][256], char[][256], int[][2], char*, int*, int*, int*);

int TelechargementDecor(char[][256],SDL_Texture**, SDL_Renderer*, int*, int*);

int TelechargementObjet(char[][256],SDL_Texture**, SDL_Renderer*, int*);

int TelechargementPlayer(char*, SDL_Texture**, SDL_Renderer*);