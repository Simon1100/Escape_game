#include "GestionDuTexte.h"
#include <SDL2/SDL_ttf.h>


//Cette fonction permet de "telecharger" les textures des différents textes
void TelechargementTexte(SDL_Texture *TextureTexte[4], char ContenuTexte[20], SDL_Renderer *renderer){
    SDL_Color blanc = {255, 255, 255};
    TTF_Font* police = TTF_OpenFont("Roboto-Thin.ttf",200);
    for (int i = 0; i < 4; i++){
        if (i == 0){ //Telecharge le texte de l'affichage du temps ! //ce fait toutes les 1 secondes !
            SDL_Surface* texte = TTF_RenderText_Solid(police, ContenuTexte, blanc);
            TextureTexte[i] = SDL_CreateTextureFromSurface(renderer, texte);
            SDL_FreeSurface(texte);
        }
        if (i == 1){ //Perdu
            SDL_Surface* texte = TTF_RenderText_Solid(police, "Perdu !", blanc);
            TextureTexte[i] = SDL_CreateTextureFromSurface(renderer, texte);
            SDL_FreeSurface(texte);
        }
        if (i == 2){ //Temps écoulé
            SDL_Surface* texte = TTF_RenderText_Solid(police, "Temps ecoule !", blanc);
            TextureTexte[i] = SDL_CreateTextureFromSurface(renderer, texte);
            SDL_FreeSurface(texte);
        }
        if (i == 3){ //Victoire
            SDL_Surface* texte = TTF_RenderText_Solid(police, "Victoire ! ", blanc);
            TextureTexte[i] = SDL_CreateTextureFromSurface(renderer, texte);
            SDL_FreeSurface(texte);
        }
    }
        if(police != NULL)
        TTF_CloseFont(police);
}

/*Cette fonction permet de mettre dans le renderer le texte attendu pour l'afficher,
  La présence d'un paramètre mode permet de faire appel à l'écriture que nous souhaitons afficher
*/
void AffichageTexte(int LargeurHauteur[2], SDL_Texture *TextureTexte[4], SDL_Renderer *renderer, const int mode){
    if (mode == 0){//Texte du temps
        SDL_Rect destination = {(31*LargeurHauteur[0]),(81*LargeurHauteur[1])-250,50,50};
        SDL_RenderCopy(renderer,TextureTexte[0],NULL,&destination); //on met dans le renderer le texte
    }
    if (mode == 1){//Texte de la défaite
        SDL_Rect destination = {(32*LargeurHauteur[0])-250,(32*LargeurHauteur[1])-250,500,500};
        SDL_RenderCopy(renderer,TextureTexte[1],NULL,&destination); //on met dans le renderer le texte
    }
    if (mode == 2){//Texte du temps écoulé
        SDL_Rect destination = {(20*LargeurHauteur[0])-270,(32*LargeurHauteur[1])-275,900,500};
        SDL_RenderCopy(renderer,TextureTexte[2],NULL,&destination); //on met dans le renderer le texte
    }
    if (mode == 3){//Texte de la victoire
        SDL_Rect destination = {(20*LargeurHauteur[0])-260,(32*LargeurHauteur[1])-275,900,500};
        SDL_RenderCopy(renderer,TextureTexte[3],NULL,&destination); //on met dans le renderer le texte
    }
    SDL_RenderPresent(renderer); //on met à jour l'écran en plaçant le contenu du renderer dessus (sans cela le contenu du renderer ne pourrait être affiché)
}

