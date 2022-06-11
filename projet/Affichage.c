#include "Affichage.h"

//Cette fonction permet de placer dans le renderer les différents images du décor, ces images ont été définies initialement par des nombres dans la fonction "ChargementDecor".
int AffichageDecor(int Position[], char Tuile[50][256],SDL_Texture *TextureDecor[50], SDL_Renderer *renderer, int LageurHauteur[2]){
    SDL_Rect destination = {0,0,64,64};  //on choisit ici le dimensionnement des textures comme des carrés de 64px*64px
    int cpt=0, erreur=0;
    for(int i=0;i<LageurHauteur[1];i++){ //On définit autant de lignes que la taille de la hauteur
        for(int j=0;j<LageurHauteur[0];j++){  //On définit le nombre de la taille de la largeur comme le nombre correspondant aux nombres d'éléments par lignes
            if(erreur!=0) return 5;
            int val = Position[cpt]; //On prend ici la valeur de la tuile correspondant à la texture à afficher (cette variable "valeur" nous donne l'emplacement de l'image recherché dans le tableau de texture)
            erreur = SDL_RenderCopy(renderer,TextureDecor[val],NULL,&destination); //On copie dans le renderer la texture (l'image) à sa position attendu
            destination.x+=64; //on incrémente de 64px sur l'axe des x (x,y) la position du rectangle qui correspond à l’endroit du renderer où doit être copié la texture.
            cpt++;
        }
        destination.x=0; //on réinitialise la position du rectangle sur l'axe des x à 0. (retour à ligne)
        destination.y+=64; //on incrémente de 64px la position du rectangle sur l'axe des y (saut de ligne)
    }
    return erreur; //on retourne la valeur de l'erreur : 0 si tout s'est bien passé sinon 5.
}

//Cette fonction permet de placer dans le renderer les différents objets du décor, ces images ont été définies initialement par des nombres dans la fonction "ChargementObjetEtPayer".
int AffichageObjet(int PositionObjet[50][2],SDL_Texture *TextureObjet[50], SDL_Renderer *renderer, int* NbObjet){
    SDL_Rect destination = {0,0,45,45}; //on choisit la dimension des objets en fonction du rendu affiché dans le sujet du projet. 45 me semble être la bonne valeur.
    int erreur = 0;
    for(int cpt=0;cpt<(*NbObjet);cpt++){
        if(erreur!=0) return 6;
        destination.x=PositionObjet[cpt][0]; //On va chercher la position de l'objet grâce aux valeurs trouvé dans le fichier et mise dans le tableau "PositionObjet"
        destination.y=PositionObjet[cpt][1]; 
        erreur = SDL_RenderCopy(renderer,TextureObjet[cpt],NULL,&destination); //On copie dans le renderer la texture (l'image) à sa position attendu
    }
    return erreur;
}

//Cette fonction permet de mettre dans le renderer le player, en conséquence l'afficher sur le jeu
void AffichagePlayer(int PositionPlayer[2], SDL_Texture *TexturePlayer, SDL_Renderer *renderer, float angle){
    SDL_Rect destination = {PositionPlayer[0],PositionPlayer[1],45,45};
    SDL_RenderCopyEx(renderer,TexturePlayer,NULL,&destination,angle,NULL,SDL_FLIP_HORIZONTAL); //on met dans le renderer le player
}
