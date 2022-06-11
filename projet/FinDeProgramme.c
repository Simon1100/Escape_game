#include "FinDeProgramme.h"
#include <SDL2/SDL_ttf.h>

//Fonction pour détruire la fenêtre, le renderer, les textures et quitter la SDL s'ils ont été utilisées.
int Supp(SDL_Texture *TexturePlayer, SDL_Texture *TextureDecor[50], SDL_Texture *TextureObjet[50], SDL_Texture *TextureTexte[4], SDL_Renderer *renderer, SDL_Window *fenetre, const int erreur){
    if(TexturePlayer != NULL)
        SDL_DestroyTexture(TexturePlayer);
    if(TextureTexte!= NULL)
        for (int i = 0; i < 4; i++)
        SDL_DestroyTexture(TextureTexte[i]);
    if(TextureDecor != NULL)
        for (int i = 0; i < 50; i++)
            SDL_DestroyTexture(TextureDecor[i]);
    if(TextureObjet != NULL)
        for (int i = 0; i < 50; i++)
            SDL_DestroyTexture(TextureObjet[i]);
    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if(fenetre != NULL)
        SDL_DestroyWindow(fenetre);
    if(erreur!=1) //Si on a réussi à ouvrir la SDL
        SDL_Quit();//On ferme la SDL.
    if(erreur!=4) //Si on a réussi à ouvrir TTF
        TTF_Quit();
    if(erreur) return erreur; //<=> if(erreur>0) return erreur;
    else return erreur; //<=> if(erreur==0) return 0;
}