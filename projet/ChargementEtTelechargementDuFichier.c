#include "ChargementEtTelechargementDuFichier.h"
#include "Affichage.h"
#include <SDL2/SDL_image.h>

//Fonction permettant de renvoyer un entier correspondant à la valeur d'un nombre ou d'un chiffre contenu dans une chaîne de caractères, avec certaines contraintes.
//La fonction prend en paramètre la chaîne de caractère dans laquelle est contenue le nombre, ainsi qu'un entier correspondant au nombre de "," à passer avant de commencer à "enregistrer" les chiffres présent dans la chaîne de caractère; (utile dans le cas du canvas présenté)
//La fonction après le début de l'enregistrement s'arrête quand la chaîne de caractère est vide, ou quand elle rencontre une virgule !
//Elle place dans un tableau Resultat tous les caractères correspondant à des chiffres
//Puis transforme la suite de ces caractères chiffres en un entier et le renvoie.
int NombreDansChaine(char Buffer[256], const int mode){
    int cpt1=0,cpt2=0, result;
    char Resultat[10];
    for(int i=0;i<mode;i++){  //On passe le nombre de "," souhaité !
        while(Buffer[cpt1]!=',') cpt1++;
        cpt1++;
    }
    while(Buffer[cpt1]!='\0' && Buffer[cpt1]!=',' && Buffer[cpt1]!='\n'){
        if(Buffer[cpt1]>='0' && Buffer[cpt1]<='9'){ //on stocke seulement les chiffres
            Resultat[cpt2]=Buffer[cpt1]; //on met les chiffres dans un tableau résultat
            cpt2++;
        }
        cpt1++;
    }
    Resultat[cpt2]='\0';
    sscanf(Resultat,"%d",&result); //On transfome la chaîne de caractère (contenant uniquement des chiffres) en entier !
    return result; // On renvoit l'entier correspondant
}

//Fonction qui permet à partir d'un mode et d'une chaîne de caractère de renvoyer un morceau de celle-ci;
//Nous allons utiliser cette fonction pour récuperer les adresses où sont situées les images
//Le mode 0 correspond au fait que l'enregistrement du morceau commence après la première virgule, et le mode 1 de même mais après ":"
void Position(char Buffer[256], char Resultat[256], const int mode){
    int cpt1=0,cpt2=0;
    if (mode==0){
        while(Buffer[cpt1]!=',') cpt1++;
        cpt1++; //on se place après la ,
    }
    if (mode==1){
        while(Buffer[cpt1]!=':') cpt1++;
        cpt1++; //on se place après les :
    }
    while(Buffer[cpt1]!='\n' && Buffer[cpt1]!='\0' && Buffer[cpt1]!=','){ //On découpe la chaine jusqu'à la fin de la ligne, ou bien à la présence d'une virgule
        Resultat[cpt2]=Buffer[cpt1];
        cpt1++;
        cpt2++;
    }
    Resultat[cpt2]='\0';
}

//Fonction permettant de stocker dans un tableau "LargeurHauteur" (tableau présent dans la fonction main) la valeur de la Hauteur et de la Largeur étant obtenue grâce à l'appel de la fonction "NombreDansChaine"
//Prend en paramètre ce tableau LargeurHauteur, ainsi qu'un type FILE, permettant de lire et trouver dans un fichier les lignes où sont écrites les hauteurs et largeurs
//Retourne 0 (pas de problème), 1 (ne trouve pas la largeur) ou 2 (ne trouve pas la hauteur) en fonction de l'erreur associée.
int ChargementLargeurHauteur(FILE* fichierPlateau,int LargeurHauteur[2]){
    char Buffer[256];
    while(fgets(Buffer, sizeof(Buffer), fichierPlateau)!=NULL){ 
        if((strncmp("LARGEUR:",Buffer,8)==0)){
            break;
        }
    } 
    if((strncmp("LARGEUR:",Buffer,7)==0)){ // "LARGEUR:" est présent dans le fichier on continue sinon on retourne 1
        LargeurHauteur[0] = NombreDansChaine(Buffer,0); //On stocke le nombre situé après "LARGEUR" dans la 1ère case de ce tableau, on appel NombreDansChaine dans le mode 0, car il n'y a pas d'informations à ignorer, un seul nombre est ici présent, on ne risque pas dans considérer un autre à sa place.
        while(fgets(Buffer, sizeof(Buffer), fichierPlateau)!=NULL){ 
            if((strncmp("HAUTEUR:",Buffer,8)==0)){
                break;
            }
        } 
        if((strncmp("HAUTEUR:",Buffer,7)==0)){
            LargeurHauteur[1] = NombreDansChaine(Buffer,0);
        }
        else return 2; //Si ne trouve pas la hauteur retourne 2
    }
    else return 1; //Si ne trouve pas la largeur retourne 1
    return 0; //Si tout se passe bien retourne 0 !
}

//Fonction permettant de charger les tuiles du jeu, prend en paramètre le fichier où lire l'adresse des images des tuiles ainsi que leur numéro associé,
//et un tableau permettant de stocker cette adresse à la case correspondante au numéro qui lui est associé. Permet de rendre le canevas plus flexible, 
//en effet on peut situer les numéros des tuiles dans le désordre (et leur adresse associée), ça fonctionnera quand même !
//Retourne 0 (pas de problème), 3 (ne trouve pas la mention "TAB:" permettant de préciser le début de l'enregistrement du placement des tuiles) ou 4 (ne trouve pas de tuile, c'est à dire leur adresse et leur numéro associé) en fonction de l'erreur associé.
int ChargementTuile(FILE* fichierPlateau,char Tuile[50][256], int* NbTuile){
    char Buffer[256];
    char Resultat[256];
    int cpt = 0;
    while(fgets(Buffer, sizeof(Buffer), fichierPlateau)!=NULL){ 
        if((strncmp("TAB:",Buffer,4)==0)){
            break;
        }
        if(strncmp("TUILE:",Buffer,6)==0){
            cpt++;
            int nbr = NombreDansChaine(Buffer, 0); //on va chercher le numéro associé à la tuile actuel.
            Position(Buffer,Resultat,0); // On prend l'adresse associé à la tuile, en appelant la fonction avec le mode 0, afin de commencer le découpage après la virgule qui sépare l'adresse du numéro
            strcpy(Tuile[nbr],Resultat); //On place l'adresse de la tuille au numéro de case du tableau associé au numéro de la tuile dans le canevas
        }
    }
    *NbTuile = cpt;
    if((strncmp("TAB:",Buffer,4)!=0)){
        return 3; //Si ne trouve pas "TAB:" retourne 3
    }
    else{
        if(cpt==0){
            return 4; //Si n'a pas trouvé de tuile retourne 4
        }
        else return 0; //Si tout se passe bien retourne 0 !
    }
}

//Fonction qui permet d'aller chercher dans le canevas l'emplacement de chaque tuile dans le jeu
//Prend en paramètre le fichier où sont précisés ces emplacements par une suite de nombres correspondant au numéro de tuile préalablement asscocié.
//Prend aussi un tableau d'entier dans lequel on stocke ces nombres correspondant aux emplacements.
//(Ainsi que le tableau LargeurHauteur afin de limiter la saisie de nombre, au nombre de tuiles à afficher). Et un pointeur NbObjet qui permet de récupérer la valeur située après "NOBJETS:" dans le main.
//Retourne 0 (pas de problème), 5 (ne trouve pas la mention "NOBJETS:" permettant de préciser le début de l'enregistrement des positions et textures des objets) ou 6 (ne trouve pas de nombre)
int ChargementDecor(FILE* fichierPlateau, int* Position, int LargeurHauteur[2], int* NbObjet){

    char Buffer[256];
    char temp[10];
    temp[0]='\0';
    int cpt1=0, cpt2=0,cpt3=0, result;
    while((fgets(Buffer, sizeof(Buffer), fichierPlateau)!=NULL)){ //Tant qu'on ne rencontre pas en début de ligne "NOBJETS" on continue, ou bien on s'arrête en fin de fichier
        if((strncmp("NOBJETS:",Buffer,8)==0)){ // && cpt3<=(LargeurHauteur[0]*LargeurHauteur[1])){  = A tendance à engendrer des petits bugs (core dumped), si l'utilisateur ne saisit pas le bon nombre de nombre, au lieu d'afficher le message d'erreur souhaité, j'ai donc décidé de ne pas le mettre
            break;
        }
        int TailleLigne = strlen(Buffer);
        if(TailleLigne==1) TailleLigne = -1;  //Si la ligne est vide = (correspond au caractère '\n'), on passe à la ligne suivante (en ignorant la boucle for qui suit)
        for(int i=TailleLigne+1;i>1;i--){
            if(Buffer[cpt1]>='0' && Buffer[cpt1]<='9'){ //on stocke le nombre actuel dans un tableau temp
                temp[cpt2]=Buffer[cpt1];
                temp[cpt2+1]='\0'; //On signale la fin de la chaîne de caractère
                cpt2++;
            }
            else{ //Si on ne rencontre pas un chiffre -> cas de la fin d'un nombre ou de la ligne
                if(temp[cpt2-1]!='\0'){ //Cas de la fin d'un nombre, on vérifie ici que le tableau temp comprend au minimum un chiffre
                    temp[cpt2]='\0';
                    sscanf(temp,"%d",&result); //On transforme la chaîne de caractère en entier
                    Position[cpt3]=result; //on place l'entier dans le tableau Position, chaque nombre rencontré est rangé dans l'ordre de rencontre dans ce tableau
                    cpt3++;
                }
                while((Buffer[cpt1]<'0' || Buffer[cpt1]>'9')&& (Buffer[cpt1]!='\n')){ //Ici on avance dans le buffer (correspondant à une ligne du fichier) jusqu'à trouver un chiffre ou bien jusqu'à la fin de fichier
                    cpt1++;
                }
                cpt1--; //On se place avant le chiffre (cpt1 correspondant à l'avancement dans le tableau Buffer), le while nous amène au cpt1 correspondant au chiffre. Pour éviter de "sauter" une case nous devons ici désincrémenter de 1.
                cpt2=0; //On se place à la 1er case du tableau temp
            }
            cpt1++;
        }
        cpt1 = cpt2 = 0; //On remet à 0 les compteurs afin de relire une nouvelle ligne.
        temp[0]='\0'; //de même pour temp.
    }

    if((strncmp("NOBJETS:",Buffer,8)!=0)){
        return 5; //Si ne trouve pas "NOBJETS:" retourne 5
    }
    else{
        *NbObjet = NombreDansChaine(Buffer,0); //permet de récuperer la valeur situé après "NOBJETS:".
        if(cpt3==0){
            return 6; //Si n'a pas trouvé de nombre retourne 6
        }
        else return 0; //Si tout se passe bien retourne 0 !
    }
}

//Fonction qui permet de charger les tuiles (=texture) des objets, ainsi que leur position et celle du player (et sa texture aussi) (=joueur)
//On prend en paramètre le fichier, les tableaux permettant d'enregistrer ces informations.
//Retourne 0 (pas de problème), 7 (ne trouve pas la mention "JOUEUR:" permettant de préciser la texture et position de celui-ci) ou 8 (ne trouve pas d'objets)
int ChargementObjetEtPlayer(FILE* fichierPlateau,char Objet[50][256], int PositionObjet[50][2],char Player[50], int PositionPlayer[2]){

    char Buffer[256];
    char Resultat[256];
    int cpt=0;
    while(fgets(Buffer, sizeof(Buffer), fichierPlateau)!=NULL){ 
        if((strncmp("JOUEUR:",Buffer,7)==0)){
            break;
        }
        if(strncmp("OBJET:",Buffer,6)==0){
            int nbr = NombreDansChaine(Buffer,0); //on cherche le numéro de l'objet
            Position(Buffer,Resultat,0); // On prend l'adresse associé à l'objet, en appelant la fonction avec le mode 0, afin de commencer le découpage après la virgule qui sépare l'adresse du numéro
            strcpy(Objet[nbr],Resultat); //On place l'adresse de l'objet au numéro de case du tableau associé au numéro de l'objet dans le canevas

            PositionObjet[nbr][0] = NombreDansChaine(Buffer,2); //On place la position x de l'objet au numéro de case du tableau "PositionObjet" associé au numéro de l'objet dans le canevas. Mode 2 pour commencer l'enregistrement du nombre après la 2ème virgule, jusqu'à la prochaine
            PositionObjet[nbr][1] = NombreDansChaine(Buffer,3); //De même pour l'axe des y (correspondant à la position de la hauteur de la fenetre) (et x la largeur).  Mode 3 pour commencer l'enregistrement du nombre après la 3ème virgule
            cpt++; //Pour savoir s'il y a eu des Objets trouvé
        }
    }
    if(cpt==0){
        return 8; //Si n'a pas trouvé d'OBJET retourne 8
    }
    else{ //On fait la même chose que sur les objets pour le Player
        if((strncmp("JOUEUR:",Buffer,7)==0)){
            Position(Buffer,Resultat,1); 
            strcpy(Player,Resultat);
            PositionPlayer[0] = NombreDansChaine(Buffer,1);
            PositionPlayer[1] = NombreDansChaine(Buffer,2);
            return 0; //Si tout se passe bien retourne 0 ! On ignore les autres caractère dans le fichier si nous ne somme pas à la fin
        }
        else return 7; //Si n'a pas trouvé le player retourne 7
    }
}

//Fonction permettant d'appeler toutes les fonctions qui permettent le chargement des textures, c'est à dire les fonctions qui lisent le fichier
//Prend en paramètres tous les tableaux que l'on doit remplir des informations de ce fichier. Et un pointeur NbObjet qui permet de récupérer la valeur située après "NOBJETS:" dans le main.
//Retourne la valeur des erreurs associée aux fonctions d'appel.
int Chargement(int LargeurHauteur[2], int** Position, char Tuile[50][256], char Objet[50][256], int PositionObjet[50][2], char Player[50], int PositionPlayer[2], int* NbObjet, int* NbTuile){
    int erreur;
    FILE* fichierPlateau;
    fichierPlateau = fopen("plateau.tab","r");
    if(fichierPlateau==NULL){
        printf("Le fichier permettant de lire le canevas du jeu n'a pas pu être ouvert.\n");
        return -1;
    }
    erreur = ChargementLargeurHauteur(fichierPlateau, LargeurHauteur);
    if(!erreur){
        *Position = (int*) malloc(sizeof(int)*LargeurHauteur[0]*LargeurHauteur[1]); //On facilite l'allocation de la mémoire, on attribuant exactement au tableau Position le nombre de case qui va devoir remplir.
        erreur = ChargementTuile(fichierPlateau, Tuile, NbTuile);
        if(!erreur){
            erreur = ChargementDecor(fichierPlateau, *Position, LargeurHauteur, NbObjet);
            if(!erreur){
                erreur = ChargementObjetEtPlayer(fichierPlateau, Objet, PositionObjet,Player,PositionPlayer);
            }
        }
    }
    fclose(fichierPlateau);
    return erreur;
}

//Cette fonction permet de "telecharger" dans un tableau de texture les textures que nous aurons besoin d'afficher au cours de ce programme
int TelechargementDecor(char Tuile[50][256],SDL_Texture *TextureDecor[50], SDL_Renderer *renderer, int LageurHauteur[2], int* NbTuile){
    int cpt=0, erreur=0;
    for(int cpt=0;cpt<(*NbTuile);cpt++){
        if(erreur!=0) return erreur;
        TextureDecor[cpt] = IMG_LoadTexture(renderer, Tuile[cpt]);  //On charge les images une par une dans le tableau de texture
        if(TextureDecor[cpt]==NULL){ //Si on ne trouve pas l'image recherchée : erreur.
            fprintf(stderr, "Erreur SDL_CreateTexture (Décor): %s", SDL_GetError());
            erreur = 5;
            continue;
        }
    }
    return erreur; //on retourne la valeur de l'erreur : 0 si tout s'est bien passé sinon 5.
}

//Cette fonction permet de "telecharger" dans un tableau de texture les textures que nous aurons besoin d'afficher au cours de ce programme
int TelechargementObjet(char objet[][256],SDL_Texture *TextureObjet[50], SDL_Renderer *renderer, int* NbObjet){
    int erreur = 0;
    for(int cpt=0;cpt<(*NbObjet);cpt++){
        if(erreur!=0) return erreur;
        TextureObjet[cpt] = IMG_LoadTexture(renderer, objet[cpt]); 
        if(TextureObjet[cpt]==NULL){ //Si on ne trouve pas l'image recherchée : erreur.
            fprintf(stderr, "Erreur SDL_CreateTexture (Objet): %s", SDL_GetError());
            erreur = 6;
            continue; 
        }
    }
    return erreur; //on retourne la valeur de l'erreur : 0 si tout s'est bien passé sinon 6.
}


//Cette fonction permet de "telecharger" la texture du player
int TelechargementPlayer(char Player[50], SDL_Texture **TexturePlayer, SDL_Renderer *renderer){
    int erreur =0;
    *TexturePlayer=IMG_LoadTexture(renderer, Player);
    if(TexturePlayer == NULL){
        fprintf(stderr, "Erreur SDL_CreateTexture (Player) : %s", SDL_GetError());
        erreur = 7;
    }
    return erreur;             
}