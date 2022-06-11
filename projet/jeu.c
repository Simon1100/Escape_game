//|___________________________________________________________________________________|//
//|Informations :                                                                     |// 
//|                                                                                   |// 
//|Pour se déplacer : Utiliser les flèches directionnelles.                           |//
//|                   Flèche du haut : Va vers où regarde le bonhomme                 |//
//|                                                                                   |//
//|Pour lancer le programme : make puis ./jeu                                         |//
//|                                                                                   |//
//|Attention : Il est possible de rentrer dans un petit peu dans un mur si une flaque |//
//|            d'eau se trouve à côté d'un mur.                                       |//
//|            Correction partielle du "bug" en commentaire                           |//
//|                                                                                   |// 
//|           Concernant le bas gauche de la carte du jeu :                           |//
//|           Il est possible que ce soit difficile d'accès (c'est au pixel près)     |//
//|           Il faut se coller au maximum possible sur le mur gauche, se coller      |//
//|           en avançant de façon diagonale, si ça ne passe pas le joueur devra      |//
//|           reculer et réessayer !                                                  |//
//|                                                                                   |//
//|                                                                                   |//
//|     Le Joueur a 2 minutes 30 pour sortir du labyrinthe s'il ne veut pas perdre !  |//
//|                                                                                   |//
//|                                                                                   |// 
//|Sources : https://wiki.libsdl.org/                                                 |//
//|          https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c    |// 
//|          https://openclassrooms.com/fr/courses/19980-apprenez-a-programmer-en-c   |//
//|          /18176-la-gestion-des-evenements                                         |//
//|___________________________________________________________________________________|//

#include "ChargementEtTelechargementDuFichier.h"
#include "Affichage.h"
#include "GestionDuTexte.h"
#include "GestionDesCollisions.h"
#include "FinDeProgramme.h"
#include <SDL2/SDL_ttf.h>

int main(){
    //On définit ici les tableaux où l'on va stocker les valeurs trouvé dans le fichier
    int LargeurHauteur[2], PositionObjet[50][2], PositionPlayer[2];
    char Tuile[50][256], Objet[50][256], Player[50];
    int *Position;

    //Variable qui permet de stocker le nombre d'objets ou tuiles présent dans le jeu
    int NbObjet = 0,NbTuile = 0;

    //Variables de gestion d'erreurs : Gestion des erreurs d'ouverture des initialisations et création de fenêtre par exemple //gestions des erreurs lors du chargement des informations présentent dans le fichier .tab
    int erreur = 0, erreurChargement = 0;

    //Déplacement du player
    float inc_angle = 0, angle = 0, inc_x = 0, inc_y = 0, x = 0, y = 0;
    const int  Vitesse = 10;
    const int  VitesseAngle = 15;

    //Nombre de collisions
    int NbCoordBordure = 0, NbCoordBordureObjet = 0;

    //Gestions des événements : Exemple : un mur bloque le joueur alors la variable état nous l'indique et nous empêchons par conséquence le déplacement du joueur
    int val =1, etat, etat1;

    //Gestion du temps
    int tempsjeu = 150000; //On définit le temps de la partie ici (2 minutes 30 !), on aurait pu aussi utiliser un la directive #define
    int temps=tempsjeu, temps2=tempsjeu-1000;
    //Pour l'affichage du temps
    char tempscarac[20];

    //Tableau de structure permettant de stoker les coordonnées des bordures qui doivent engendrer des collisions (et le numéro de collision)
    //Je leur attribut de grandes tailles pour être sûr d'avoir assez de place
    Collision Bordure[20000];
    Collision BordureObjet[20000];

    //Je déclare les variables utile pour gérer les évènements
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;

//------------------------------------------------------------------------------------------------------------------------------//


    //Chargement des informations contenue dans le fichier
    erreurChargement = Chargement(LargeurHauteur, &Position, Tuile, Objet, PositionObjet,Player,PositionPlayer,&NbObjet,&NbTuile); //On appel ici la fonction qui va charger dans les tableaux défini plus haut les valeurs trouver dans le fichier. Nous renvoie l'erreur associé s'il y a, sinon 0
    
    //On appelle les fonctions permettant de stocker dans les structures "Collision" les coordonnées des bordures des murs et objets (caisses)
    RemplirBordure(Position,Tuile,LargeurHauteur,Bordure,&NbCoordBordure);
    RemplirBordureObjet(PositionObjet,Objet,&NbObjet,BordureObjet,&NbCoordBordureObjet);

    if(!erreurChargement){
        SDL_Window *fenetre = NULL; //on crée un pointeur vers SDL_Window (une structure de la SDL qui représente la fenêtre).
        SDL_Renderer *renderer = NULL; //on crée un pointeur vers SDL_Renderer (c'est une structure qui représente le renderer).
        //on crée les textures
        SDL_Texture *TextureDecor[50];
        SDL_Texture *TextureObjet[50]; 
        SDL_Texture *TexturePlayer;
        SDL_Texture *TextureTexte[4];

        if(0 != SDL_Init(SDL_INIT_VIDEO)) //On initialise le système de gestion de rendu de la SDL.
        {
            fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError()); //on affiche l'erreur correspondante si l'initialisation de la SDL n'a pas pu se faire correctement.
            erreur = 1;
        }
        if(!erreur){ //si les lignes précédentes ne génèrent pas d'erreurs on continue ! (on vérifie cette condition plusieurs fois afin d'aller directement à la fin du programme en cas d'erreur)
            fenetre = SDL_CreateWindow("Jeu",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,LargeurHauteur[0]*64,LargeurHauteur[1]*64,SDL_WINDOW_SHOWN); //création de la fenêtre, le flag  SDL_WINDOW_SHOWN peut être remplacé par 0, il est déjà défini par défaut. On centre la fenêtre sur le flux de sortie = écran. On applique les proportions de notre fenêtre et de nos images en multipliant par 64 (longueur en px de nos carrés d'images),de chaque côté aux nombres de carrés qu'il y aura. Sans cela, il n'y aurait aucunes proportions.
            if(fenetre == NULL){
                fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
                erreur = 2;
            }
            if(!erreur){
                renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED); //si le renderer ne peut pas être crée en utilisant l'accélération matérielle, il essayera avec le drapeau SDL_RENDERER_SOFTWARE (rendu effectué par le CPU et non le GPU) grâce à l'indication de l'index -1. Si les deux rendus ne sont pas possible -> affichage d'une erreur -> on détruit la fenêtre et on ferme la SDL.
                if(renderer == NULL){
                    fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
                    erreur = 3;
                }
                if(!erreur){
                    if(TTF_Init() == -1){
                        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
                        erreur = 4;
                    }
                    if(!erreur){
                        //On appelle ici les quatres fonctions qui permettent de "telecharger" les différentes textures que nous afficheront par la suite
                        erreur = TelechargementPlayer(Player, &TexturePlayer, renderer);
                        erreur = TelechargementObjet(Objet, TextureObjet, renderer, &NbObjet);
                        erreur = TelechargementDecor(Tuile, TextureDecor, renderer, LargeurHauteur,&NbTuile);
                        TelechargementTexte(TextureTexte, "150", renderer);

                        //On lit cette boucle jusqu'à la victoire ou la défaite du joueur, ou bien si le joueur décide de fermer la fenêtre
                        while(!quit){
                            temps = tempsjeu - SDL_GetTicks(); //On regarde depuis combien de temps est ouvert le programme
                            if(temps<=temps2){
                                sprintf(tempscarac, "%d", temps/1000); //On convertit toutes les 1 secondes le temps actuelle en texte
                                TelechargementTexte(TextureTexte, tempscarac, renderer); //Toutes les 1 secondes, on telecharge la nouvelle texture du temps
                                temps2-=1000;
                            }

                            if(temps>0){ //Le joueur à 2 minutes 30 pour gagner la partie ! Sinon il perd !
                                //S'il y a un événement = fermeture du programme, clique de souris alors la condition du si est vérifié
                                //Et si c'est un événement attendu alors on agit en conséquence : déplacement du joueur par exemple si l'utilsateur clique sur les flèches directionnelles
                                if(SDL_PollEvent(&event)){
                                    switch(event.type){
                                        case SDL_KEYDOWN: //Evénement concernant l'appuie des touches
                                            switch (event.key.keysym.sym){   //On récupère la touche "virtuel":(a équivaut à q de manière physique sur un clavier azerty, on demande quand on clique sur a de récupérer a et non q) saisie.
                                                case SDLK_DOWN: //Flèche du bas
                                                    inc_y=-Vitesse*sin((angle+90)/180*M_PI); //j'ai du un tout petit peu changer la formule, (+90 pour s'aligner au niveau de où regarde le player)
                                                    inc_x=Vitesse*cos((angle+90)/180*M_PI);
                                                    //Avant d'ordonner l'instruction d'avancer on vérifie s'il y a une tuile qui engendre un événement (Mur qui bloque, trou, flaque..) et un objet devant la direction du joueur
                                                    //La condition du If ci-dessous est vrai si dans la direction du joueur il n'y a aucune collision de mur
                                                    if((etat = VerifBordure(Bordure,PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y,&NbCoordBordure))){
                                                        val = VerifBordureObjet(BordureObjet, Bordure, PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y, PositionObjet, &NbCoordBordureObjet, &NbCoordBordure,LargeurHauteur, Position, Tuile, Objet, Vitesse);
                                                        //Si un Objet ne bloque pas la direction du joueur on avance ! (Si il y a une caisse devant le joueur et que cette caisse n'est pas bloqué par une collision on avance la caisse (on fait ça dans le fonction VerifBordureObjet() et pour le joueur (ci-dessous) )
                                                        if(val ==1){
                                                            PositionPlayer[1]+=inc_y;
                                                            PositionPlayer[0]+=inc_x;
                                                        }
                                                        //Si le joueur rencontre un trou ou du feu, on affiche le texte perdu (indiqué par le mode 1 dans la fonction AffichageTexte()) pendant 5 secondes et ensuite on quitte le programme
                                                        if(etat==2 || etat==3){
                                                            quit=1;
                                                            AffichageTexte(LargeurHauteur, TextureTexte, renderer, 1);
                                                            SDL_Delay(5000);
                                                        }
                                                        if(etat==4){ //Si on rencontre une flaque, on fait faire avancer le joueur juqu'à un mur, du feu ou un trou (si on rencontre du feu et un trou le joueur brule ou tombe dans le trou, et donc on affiche "Perdu")
                                                            while((etat1 = VerifBordure(Bordure,PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y,&NbCoordBordure)) && VerifBordureObjet(BordureObjet, Bordure, PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y, PositionObjet, &NbCoordBordureObjet, &NbCoordBordure,LargeurHauteur, Position, Tuile, Objet, Vitesse)){
                                                                if((etat1 != 2) && (etat1!= 3)){
                                                                    PositionPlayer[1]+=inc_y;
                                                                    PositionPlayer[0]+=inc_x;

                                                                    /*Correction du bug partiel (ne corrige pas bien le problème : j'ai donc décidé de ne pas l'incorporer) qui fait faire rentrer le joueur dans un mur, quand une flaque est situé à côté d'un mur.
                                                                    if((etat1==4) && (!VerifBordure(Bordure,PositionPlayer[0]+(inc_x*2),PositionPlayer[1]+(inc_y*2),&NbCoordBordure) || !VerifBordureObjet(BordureObjet, Bordure, PositionPlayer[0]+(inc_x*2),PositionPlayer[1]+(inc_y*2), PositionObjet, &NbCoordBordureObjet, &NbCoordBordure,LargeurHauteur, Position, Tuile, Objet, Vitesse))){
                                                                        break;
                                                                    }*/

                                                                    //On affiche au fil de la glissage le joueur qui avance ! 
                                                                    AffichageDecor(Position, Tuile, TextureDecor, renderer, LargeurHauteur);
                                                                    AffichageObjet(PositionObjet, TextureObjet, renderer, &NbObjet);
                                                                    AffichagePlayer(PositionPlayer, TexturePlayer, renderer, inc_angle);
                                                                    SDL_RenderPresent(renderer); //on met à jour l'écran en plaçant le contenu du renderer dessus (sans cela le contenu du renderer ne pourrait être affiché)                      
                                                                    SDL_Delay(40); 
                                                                }
                                                                else{ //si on rencontre du feu ou un trou le joueur brule ou tombe dans le trou, et donc on affiche "Perdu"
                                                                    quit=1;
                                                                    AffichageTexte(LargeurHauteur, TextureTexte, renderer,1);
                                                                    SDL_Delay(5000);
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        if(etat==5){ //On rentre une sortie = VICTOIRE !
                                                            quit=1;
                                                            AffichageTexte(LargeurHauteur, TextureTexte, renderer,3);
                                                            SDL_Delay(5000);
                                                            break;
                                                        }
                                                    }
                                                    break;
                                                    //on pivote ici le joueur vers sa gauche ou sa droite
                                                case SDLK_RIGHT://Flèche droite
                                                        inc_angle+=VitesseAngle;
                                                    break;
                                                case SDLK_LEFT: //Flèche gauche
                                                        inc_angle-=VitesseAngle;
                                                    break;
                                                case SDLK_UP://Flèche du haut
                                                    inc_y=Vitesse*sin((angle+90)/180*M_PI); //j'ai du un tout petit peu changer la formule, (+90 pour s'aligner au niveau de où regarde le player)
                                                    inc_x=-Vitesse*cos((angle+90)/180*M_PI);
                                                    //Avant d'ordoner l'instruction d'avancer on vérifie s'il y a une tuile qui engendre un événement (Mur qui bloque, trou, flaque..) et un objet devant la direction du joueur
                                                    //La condition du If ci-dessous est vrai si dans la direction du joueur il n'y a aucune collision de mur
                                                    if((etat = VerifBordure(Bordure,PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y,&NbCoordBordure))){
                                                        val = VerifBordureObjet(BordureObjet, Bordure, PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y, PositionObjet, &NbCoordBordureObjet, &NbCoordBordure,LargeurHauteur, Position, Tuile, Objet, Vitesse);
                                                        //Si un Objet ne bloque pas la direction du joueur on avance ! (Si il y a une caisse devant le joueur et que cette caisse n'est pas bloqué par une collision on avance la caisse (on fait ça dans le fonction VerifBordureObjet() et pour le joueur (ci-dessous) )
                                                        if(val ==1){
                                                            PositionPlayer[1]+=inc_y;
                                                            PositionPlayer[0]+=inc_x;
                                                        }
                                                        //Si le joueur rencontre un trou ou du feu, on affiche le texte perdu (indiqué par le mode 1 dans la fonction AffichageTexte()) pendant 5 secondes et esuite on quitte le programme
                                                        if(etat==2 || etat==3){
                                                            quit=1;
                                                            AffichageTexte(LargeurHauteur, TextureTexte, renderer,1);
                                                            SDL_Delay(5000);
                                                        }
                                                        if(etat==4){ //Si on rencontre une flaque, on fait faire avancer le joueur juqu'à un mur, du feu ou un trou (si on rencontre du feu et un trou le joueur brule ou tombe dans le trou, et donc on affiche "Perdu")
                                                            while((etat1 = VerifBordure(Bordure,PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y,&NbCoordBordure)) && VerifBordureObjet(BordureObjet, Bordure, PositionPlayer[0]+inc_x,PositionPlayer[1]+inc_y, PositionObjet, &NbCoordBordureObjet, &NbCoordBordure,LargeurHauteur, Position, Tuile, Objet, Vitesse)){
                                                                if((etat1 != 2) && (etat1!= 3)){
                                                                    PositionPlayer[1]+=inc_y;
                                                                    PositionPlayer[0]+=inc_x;
                                                                    
                                                                    /*Correction du bug partiel (ne corrige pas bien le problème : j'ai donc décidé de ne pas l'incorporer) qui fait faire rentrer le joueur dans un mur, quand une flaque est situé à côté d'un mur.
                                                                    if((etat1==4) && (!VerifBordure(Bordure,PositionPlayer[0]+(inc_x*2),PositionPlayer[1]+(inc_y*2),&NbCoordBordure) || !VerifBordureObjet(BordureObjet, Bordure, PositionPlayer[0]+(inc_x*2),PositionPlayer[1]+(inc_y*2), PositionObjet, &NbCoordBordureObjet, &NbCoordBordure,LargeurHauteur, Position, Tuile, Objet, Vitesse))){
                                                                        break;
                                                                    }*/

                                                                    //On affiche au fil de la glissage le joueur qui avance !
                                                                    AffichagePlayer(PositionPlayer, TexturePlayer, renderer, inc_angle);
                                                                    AffichageDecor(Position, Tuile, TextureDecor, renderer, LargeurHauteur);
                                                                    AffichageObjet(PositionObjet, TextureObjet, renderer, &NbObjet);
                                                                    AffichagePlayer(PositionPlayer, TexturePlayer, renderer, inc_angle);
                                                                    SDL_RenderPresent(renderer); //on met à jour l'écran en plaçant le contenu du renderer dessus (sans cela le contenu du renderer ne pourrait être affiché)                      
                                                                    SDL_Delay(40);
                                                                }
                                                                else{ //si on rencontre du feu ou un trou le joueur brule ou tombe dans le trou, et donc on affiche "Perdu"
                                                                    quit=1;
                                                                    AffichageTexte(LargeurHauteur, TextureTexte, renderer, 1);
                                                                    SDL_Delay(5000);
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                        if(etat==5){ //On rentre une sortie = VICTOIRE !
                                                            quit=1;
                                                            AffichageTexte(LargeurHauteur, TextureTexte, renderer,3);
                                                            SDL_Delay(5000);
                                                            break;
                                                        }
                                                    }
                                                    break;
                                            }
                                            break;
                                        case SDL_QUIT:
                                            quit = SDL_TRUE;
                                            break;
                                    }
                                    SDL_Delay(20);
                                }
            
                                /*Gestion de l'angle du joueur
                                On laisse l'angle dans l'intervale [0,360]*/
                                if(inc_angle<=0){
                                    inc_angle=inc_angle+360;
                                }
                                if(inc_angle>360){
                                    inc_angle=inc_angle-360;
                                }
                                angle=abs(360-inc_angle); //Change indirectement la formule qui nous a été fourni pour calculer le déplacement du joueur

                                erreur = AffichageDecor(Position, Tuile, TextureDecor, renderer, LargeurHauteur); //on place les images à afficher dans le renderer. erreur = 0 si pas erreur sinon 5.
                                if(!erreur){
                                erreur = AffichageObjet(PositionObjet, TextureObjet, renderer, &NbObjet); //on place les images des objets à afficher dans le renderer. erreur = 0 si pas erreur sinon 6.
                                    if(!erreur){
                                        AffichagePlayer(PositionPlayer, TexturePlayer, renderer, inc_angle);
                                        AffichageTexte(LargeurHauteur, TextureTexte, renderer, 0);
                                        SDL_RenderPresent(renderer); //on met à jour l'écran en plaçant le contenu du renderer dessus (sans cela le contenu du renderer ne pourrait être affiché)                      
                                    }
                                }
                            }
                            //Si le temps est écoulé c'est à dire, que le programme fonctionne depuis 2 minutes et 30 secondes, le joueur a perdu !
                            else{
                                quit=1;
                                AffichageTexte(LargeurHauteur, TextureTexte, renderer, 2);
                                SDL_Delay(5000);
                            }
                        }
                    }
                }
            }
        }
        free(Position);
        erreur = Supp(TexturePlayer, TextureDecor, TextureObjet, TextureTexte, renderer, fenetre, erreur);
    }
    else{
        switch (erreurChargement){ //switch permettant d'afficher les erreurs retourné par les fonctions qui cherchent des informations dans le fichier
            case 1: fprintf(stderr, "Erreur : La largeur n'a pas été trouvé dans le fichier");
            break;
            case 2: fprintf(stderr, "Erreur : La hauteur n'a pas été trouvé dans le fichier");
            break;
            case 3: fprintf(stderr, "Erreur : La mention ""TAB:"" n'a pas été trouvé dans le fichier");
            break;
            case 4: fprintf(stderr, "Erreur : Aucune tuile n'a été trouvé dans le fichier");
            break;
            case 5: fprintf(stderr, "Erreur : La mention ""NOBJETS:"" n'a pas été trouvé dans le fichier"); //Ne s'affichera pas si cette erreur à lieu, fonctionnalité retiré afin d'être sûr que ça fonctionne sur toutes les machines (affiche à la place un core dumped)
            break;
            case 6: fprintf(stderr, "Erreur : Aucun nombre suivant la mention ""TAB:"" n'a été trouvé dans le fichier");
            break;
            case 7: fprintf(stderr, "Erreur :  La mention ""JOUEUR:"" n'a pas été trouvé dans le fichier");
            break;
            case 8: fprintf(stderr, "Erreur : Aucun objet n'a été trouvé dans le fichier");
            break;
            default:  
                fprintf(stderr, "Une erreur a été rencontrée, le programme va s'arrêter.");
            break;
        }
    }
    if(erreur || erreurChargement) return EXIT_FAILURE;
    else return EXIT_SUCCESS;
}