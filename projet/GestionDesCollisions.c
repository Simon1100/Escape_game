#include "GestionDesCollisions.h"
#include <string.h>

//Fonction permettant de retourner le numéro de tuile d'une certaine texture en fonction de son adresse(=représenté par du texte) dans les dossiers du jeu 
//Pour comprendre l'utilité (fonction RemplirBordure) : Savoir si à une position donné la tuile est un mur ou non, permet de trouver la localisation de toutes les tuiles recherchées afin de définir les coordonnées à laquelle il y aura une collision ou non
int NumDecor(char *texte, char Tuile[50][256]){
    int cpt = 0;
    while(strcmp(texte, Tuile[cpt])!=0 || cpt>=49) cpt++;
    return cpt;
}

//Fonction qui permet de rentrer dans un tableau d'une structure Collision, les coordonnées à laquelle celle-ci ont lieu
//Par exemple on va ici chercher toutes les positions des tuiles "mur" et définir à l'aide de leur position les coordonnées dans les tuiles qui doivent générer des collisions
void RemplirBordure(int Position[],char Tuile[50][256],int LargeurHauteur[2], Collision *Bordure, int *NbCoordBordure){
    /*Le tableau CoordonneesActuelle s'incrémente de façon à donner la position exacte des coordonnées de chaque tuile
        Par exemple la tuile numéro 0 à pour coordonnées 0,0. Numéro 1 : 64,0. Numéro 16 : 0,64
    */
    int cpt = 0, CoordonneesActuelle[2] = {0, 0};
    for(int i=0;i<LargeurHauteur[1];i++){
        for(int j=0;j<LargeurHauteur[0];j++){  //On parcourt ici les positions de chaque tuile une par une afin de trouver les textures recherchées
            /*Pour les coordonnées on donne la position (x,y) du haut gauche d'un rectangle et les coordonnées (x,y) du bas droit du rectangle
                Chaque collision est en effet représenté par 2 coordonnées, pour donner une image de cette représentation, je dessine ici virtuellement un rectangle, à l'aide de 2 coordonnées (une coordonnée = (x,y))
                Dans le tableau Bordure nous accolons donc ces 2 coordonnées, ils se suivront dans le tableau. Le tableau Bordure fonctionne donc en pair. (Comme de l'adressage pair en assembleur par exemple...)
                Ceci justifie la présente du +1 : "Bordure[cpt+1].?"

                .NBCollision permet de donner un numéro, qui permettra par la suite de savoir avec quel "tuile" la collision a eu lieu, et d'agir en conséquence
            */
            //Ici on donne les coordonnées de la texture "mur_gauche" et "mid_gauche"(=dans son cas on ignore le petit morceau de "bois" qui dépasse = pas très utile, toujours rattaché par une texture "mur_gd") qui doivent engendrer des collisions. (ces deux texture génère les mêmes collisions)
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_gauche.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mid_gauche.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+48; //+48 pour prendre en compte la collissions à partir de la fin de l'affichage du "bois"
                Bordure[cpt].y = CoordonneesActuelle[1];
                Bordure[cpt+1].x = CoordonneesActuelle[0]+48;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+64;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 1; //1 pour mur gauche
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_bas.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mid_bas.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0];
                Bordure[cpt].y = CoordonneesActuelle[1]+16; //+16 pour se mettre à la hauteur du "bois"
                Bordure[cpt+1].x = CoordonneesActuelle[0]+64;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+16;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 2; //2 pour mur bas
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_haut.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mid_haut.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0];
                Bordure[cpt].y = CoordonneesActuelle[1]+48;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+64;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+48;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 3; //3 pour mur haut
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_droit.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mid_droit.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+16;
                Bordure[cpt].y = CoordonneesActuelle[1];
                Bordure[cpt+1].x = CoordonneesActuelle[0]+16;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+64;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 4; //4 pour mur droit
                (*NbCoordBordure)++;
                cpt+=2;
            }
           /*A noter ici on "découpe" les morceaux sur 2 pixels de largeur ou hauteur, afin de permettre aux joueur de passer ses épaules au-dessus de ces textures

                ATTENTION
                EN EFFET LE PLAYER PEUT PASSER AU DESSUS DES MURS HORIZONTAUX ET VERTICAUX,
                NOUS VOYONS D'AU-DESSUS, IMAGINONS QUE LES EPAULES DU PLAYER SOIENT PLUS GRANDES QUE LA HAUTEURS DE CES MURS,
                DANS CE CAS ON PEUT DEVINER QUE LE PLAYER VOIT AU-DESSUS DES MURS
            
            */
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_hb.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+32;
                Bordure[cpt].y = CoordonneesActuelle[1];
                Bordure[cpt+1].x = CoordonneesActuelle[0]+33;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+64;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 5; //5 mur vertical
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_gd.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0];
                Bordure[cpt].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+64;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+33;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 6; //6 pour mur horizontal
                (*NbCoordBordure)++;
                cpt+=2;
            } 
            /*A partir d'ici on fait la même chose mais on "découpe" deux fois une texture, on considérant une partie d'un morceau comme un mur
                horizontal et l'autre un mur vertical (d'où l'utilisations des mêmes numéros de collisions).
            */
           if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_angle_db.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+32;
                Bordure[cpt].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+33;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+64;
                Bordure[cpt+2].x = CoordonneesActuelle[0]+32;
                Bordure[cpt+2].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+3].x = CoordonneesActuelle[0]+64;
                Bordure[cpt+3].y = CoordonneesActuelle[1]+33;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 5;
                Bordure[cpt+2].NbCollision = Bordure[cpt+3].NbCollision = 6;
                *NbCoordBordure=*NbCoordBordure+2;
                cpt+=4;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_angle_dh.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+32;
                Bordure[cpt].y = CoordonneesActuelle[1];
                Bordure[cpt+1].x = CoordonneesActuelle[0]+33;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+2].x = CoordonneesActuelle[0]+32;
                Bordure[cpt+2].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+3].x = CoordonneesActuelle[0]+64;
                Bordure[cpt+3].y = CoordonneesActuelle[1]+33;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 5;
                Bordure[cpt+2].NbCollision = Bordure[cpt+3].NbCollision = 6;
                *NbCoordBordure=*NbCoordBordure+2;
                cpt+=4;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_angle_gb.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+32;
                Bordure[cpt].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+33;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+64;

                Bordure[cpt+2].x = CoordonneesActuelle[0];
                Bordure[cpt+2].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+3].x = CoordonneesActuelle[0]+32;
                Bordure[cpt+3].y = CoordonneesActuelle[1]+33;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 5;
                Bordure[cpt+2].NbCollision = Bordure[cpt+3].NbCollision = 6;
                *NbCoordBordure=*NbCoordBordure+2;
                cpt+=4;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/mur_angle_gh.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+32;
                Bordure[cpt].y = CoordonneesActuelle[1];
                Bordure[cpt+1].x = CoordonneesActuelle[0]+33;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+2].x = CoordonneesActuelle[0];
                Bordure[cpt+2].y = CoordonneesActuelle[1]+32;
                Bordure[cpt+3].x = CoordonneesActuelle[0]+32;
                Bordure[cpt+3].y = CoordonneesActuelle[1]+33;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 5;
                Bordure[cpt+2].NbCollision = Bordure[cpt+3].NbCollision = 6;
                *NbCoordBordure=*NbCoordBordure+2;
                cpt+=4;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/trou.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+8;
                Bordure[cpt].y = CoordonneesActuelle[1]+8;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+48;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+48;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 7; //7 pour le trou
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/feu.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+8;
                Bordure[cpt].y = CoordonneesActuelle[1]+8;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+48;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+48;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 8; //8 pour le feu
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/flaque.png",Tuile)){
                Bordure[cpt].x = CoordonneesActuelle[0]+12;
                Bordure[cpt].y = CoordonneesActuelle[1]+12;
                Bordure[cpt+1].x = CoordonneesActuelle[0]+52;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+52;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 9; //9 pour la flaque
                (*NbCoordBordure)++;
                cpt+=2;
            }
            if((Position[i*LargeurHauteur[0]+j])==NumDecor("../images/sortie_bas.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/sortie_droite.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/sortie_gauche.png",Tuile) || (Position[i*LargeurHauteur[0]+j])==NumDecor("../images/sortie_haut.png",Tuile)){
                
                
                
                
                
                
                Bordure[cpt].x = CoordonneesActuelle[0];
                Bordure[cpt].y = CoordonneesActuelle[1];
                Bordure[cpt+1].x = CoordonneesActuelle[0]+64;
                Bordure[cpt+1].y = CoordonneesActuelle[1]+64;
                Bordure[cpt].NbCollision = Bordure[cpt+1].NbCollision = 10; //10 pour les sorties
                (*NbCoordBordure)++;
                cpt+=2;
            }
            CoordonneesActuelle[0]+=64;
        }
        //Permet d'actualiser les coordonnées actuelles de chaque tuile, même principe que la fonction AffichageDecor
        CoordonneesActuelle[0]=0;
        CoordonneesActuelle[1]+=64;
    }
}

//Fonction qui permet de rentrer dans un tableau d'une structure Collision, les coordonnées à laquelle celle-ci ont lieu
//Même principe de fonction que RemplirBordure mais pour les objets cette fois-ci !
void RemplirBordureObjet(int PositionObjet[50][2],char Objet[50][256],int *NbObjet, Collision *BordureObjet, int *NbCoordBordureObjet){
    int cpt=0;
    for(int i=0;i<(*NbObjet);i++){
        if(strcmp("../images/caisse.png",Objet[i])==0){
            BordureObjet[cpt].x=PositionObjet[i][0];
            BordureObjet[cpt].y=PositionObjet[i][1];
            BordureObjet[cpt+1].x=PositionObjet[i][0]+45;
            BordureObjet[cpt+1].y=PositionObjet[i][1]+45;
            BordureObjet[cpt].NbCollision = BordureObjet[cpt+1].NbCollision = i;
            (*NbCoordBordureObjet)++;
            cpt=cpt+2;
        }
    }
}

/*Fonction permettant de vérifier en fonction du x et y donné (représentant la coordonnée haut gauche) de l'objet ou player s'il rentre
    en collisions avec une bordure ou tuile qui engendre un "événement".
    Par exemple : vérifier si j'entre en collision avec le trou, un mur. 
    La fonction retourne un entier qui permettra d'indiquer ce qu'il faut faire en fonction de la valeur retourné,
    Par exemple : si retourne 0 le joueur ou l'objet est entré en collision avec un mur de ce fait on doit empecher l'objet ou le joueur d'avancer dans la direction du mur
                  si retourne 2 le joueur ou l'objet est entré en collision avec un trou : le joueur perd, l'objet (la caisse) bouche le trou
*/
int VerifBordure(Collision *Bordure, const int x, const int y,int *NbCoordBordure){
    //Les objets (caisses) et player font 45*45 donc on ajoute 45 à x2 et Y2 pour représenter virtuellement les coordonnées de ce carré de player ou de caisse.
    int x1 = x, y1 = y, x2 = x+45, y2 = y+45;
    int cpt=0, rep = 1;

    for (int i = 0; i < (*NbCoordBordure)*2; i=i+2){
        //Vérification de collision entre les coordoonées du player ou l'objet (saisie en paramètre) et les coordonnées de Colission du tableau Bordure.


        //Vérifications Collisions avec les murs :

        if((Bordure[i].NbCollision==1) && x1<=Bordure[i].x && ((Bordure[i].y<=y1 && y1<=Bordure[i+1].y) || (Bordure[i].y<=y2 && y2<=Bordure[i+1].y))){
            rep=0;
        }
        if((Bordure[i].NbCollision==2) && y2>=Bordure[i].y && ((Bordure[i].x<=x1 && x1<=Bordure[i+1].x) || (Bordure[i].x<=x2 && x2<=Bordure[i+1].x))){
            rep=0;
        }
        if((Bordure[i].NbCollision==3) && y1<=Bordure[i].y && ((Bordure[i].x<=x1 && x1<=Bordure[i+1].x) || (Bordure[i].x<=x2 && x2<=Bordure[i+1].x))){
            rep=0;
        }
        if((Bordure[i].NbCollision==4) && x2>=Bordure[i].x && ((Bordure[i].y<=y1 && y1<=Bordure[i+1].y) || (Bordure[i].y<=y2 && y2<=Bordure[i+1].y))){
            rep=0;
        }
        if((Bordure[i].NbCollision==5) && x1<=Bordure[i].x && Bordure[i].x<=x2  && ((Bordure[i].y<=y1 && y1<=Bordure[i+1].y) || (Bordure[i].y<=y2 && y2<=Bordure[i+1].y))){

            rep=0;
        }
        if((Bordure[i].NbCollision==6) && y1<=Bordure[i].y  && Bordure[i].y<=y2 && ((Bordure[i].x<=x1 && x1<=Bordure[i+1].x) || (Bordure[i].x<=x2 && x2<=Bordure[i+1].x))){
            rep=0;
        }

        //Vérifications Collisions avec les autres tuiles : 


        /*

        POUR LES VERIFICATIONS QUI VONT SUIVRES J'AI UTILISE UN ENCHAINEMENT DE CONDITION DANS UN IF RECUPERE SUR :
        https://developer.mozilla.org/fr/docs/Games/Techniques/2D_collision_detection     :

                            "var rect1 = {x: 5, y: 5, width: 50, height: 50}
                            var rect2 = {x: 20, y: 10, width: 10, height: 10}

                            if (rect1.x < rect2.x + rect2.width &&
                            rect1.x + rect1.width > rect2.x &&
                            rect1.y < rect2.y + rect2.height &&
                            rect1.height + rect1.y > rect2.y) {
                            // collision détectée !
                            }"

        CE QUE CA FAIT :

        "Collision entre deux rectangles alignés sur les mêmes axes (c’est-à-dire sans rotation)" :
        "L’algorithme suivant fonctionne en vérifiant qu’il n’y a pas d’espace vide entre les 4 côtés
         du rectangle. Si l’ensemble du rectangle est entouré de vide, on en conclut qu’il n’y a pas de collision."

        */

        if((Bordure[i].NbCollision==7) && (x1 < Bordure[i].x + (Bordure[i+1].x-Bordure[i].x)) && (x1 + (x2-x1) > Bordure[i].x) && (y1 < Bordure[i].y  + (Bordure[i+1].y-Bordure[i].y)) && (y1 + (y2-y1) > Bordure[i].y)){
            rep=2; //Trou
        }
        if((Bordure[i].NbCollision==8) && (x1 < Bordure[i].x + (Bordure[i+1].x-Bordure[i].x)) && (x1 + (x2-x1) > Bordure[i].x) && (y1 < Bordure[i].y  + (Bordure[i+1].y-Bordure[i].y)) && (y1 + (y2-y1) > Bordure[i].y)){
            rep=3; //Feu
        }
        if((Bordure[i].NbCollision==9) && (x1 < Bordure[i].x + (Bordure[i+1].x-Bordure[i].x)) && (x1 + (x2-x1) > Bordure[i].x) && (y1 < Bordure[i].y  + (Bordure[i+1].y-Bordure[i].y)) && (y1 + (y2-y1) > Bordure[i].y)){
            rep=4; //Flaque
        }
        if((Bordure[i].NbCollision==10) && (x1 < Bordure[i].x + (Bordure[i+1].x-Bordure[i].x)) && (x1 + (x2-x1) > Bordure[i].x) && (y1 < Bordure[i].y  + (Bordure[i+1].y-Bordure[i].y)) && (y1 + (y2-y1) > Bordure[i].y)){
            rep=5; //Les sorties
        }
    }
    return rep;
}

//Fonction permettant de boucher le trou
/* Pour ce faire, on récupère les coordonnées de la caisse qui est rentré en collision avec le trou.
    Avec ces coordonnées je vérifie si à la position des coordonnées de la caisse il y un trou, si c'est le cas
    on remplit cette position d'une tuile (le trou disparait) (cas 1). //La seule façon que j'ai trouvé pour boucher facilement le trou est de le remplacer par une tuile.
    Si ce n'est pas le cas, on remplit la position d'à cotê d'une tuile (cette position est le trou). (cas 2)


Exemple avec schéma. Si une caisse monte vers le trou et sa position x (x qui correpond à l'extrémité droite de la caisse)
pointe vers le trou alors on remplit la condition du if. (cas 1)
Sinon on doit regarder à x+45 (extrémité gauche de la caisse) : correspond au else. (cas 2)
    (Parce le x de l'extrémité droite ne pointe pas vers le trou)

Par la droite                                  Par la gauche
    ------------                                ------------                               
    |          |                                |          |
    |          |                                |          |
    |   Trou   |                                |   Trou   |
    |          |                                |          |
    |          |                                |          |
    ------------                                ------------
            ^                              ^
            |                              |
            ----------                     ----------
            |        |                     |        |
            | caisse |                     | caisse |
            |        |                     |        |
            ----------                     ----------


    Même principe pour les 4 cas : la caisse vient du haut, bas, droite, gauche.
*/
void RemplirTrou(int x, int y, int LargeurHauteur[2], int Position[], char Tuile[50][256], const int mode){

    if((Position[((x/64)+((y/64)*LargeurHauteur[0]))])==NumDecor("../images/trou.png",Tuile)){ //cas 1
        Position[((x/64)+((y/64)*LargeurHauteur[0]))]=NumDecor("../images/tuile.png",Tuile);
    }
    else{ //cas 2
        switch (mode){
            case 1:
                x=x+45;
                break;
            case 2:
                y=y+45;
                break;
        }
        Position[((x/64)+((y/64)*LargeurHauteur[0]))]=NumDecor("../images/tuile.png",Tuile);
    }

}

//fonction qui simule la destruction d'une caissse, pour ce faire on l'affiche à des coordonnées que l'utilisateur ne peut pas voir
void DetruireLaCaisse(int PositionObjet[50][2], Collision BordureObjet[50], const int i){
    PositionObjet[i/2][0]-=1000; //On affiche la caisse à des coordonnées que l'utilisateur ne peut pas voir
    BordureObjet[i].x-=1000;   // On fait de même avec les bordures (Cette caissse ne doit plus pouvoir engendrer de collisions)
    BordureObjet[i+1].x-=1000;
}

/*Fonction permettant de vérifier en fonction du x et y donné (représentant la coordonnée haut gauche) du player s'il rentre
    en collisions avec un objet qui engendre un "événement".

    ET AGIT en foncton de cet événement :
        Si le joueur percute un objet (ici des caisses) :
            -etat = 1 : S'il n'y pas de mur qui empêche d'avancer alors l'objet avance avec le joueur dans le sens où il le pousse
            -etat = 2 : Si l'objet percute un trou (lorsque le joueur essaie de pousser l'objet), alors on bouche ce trou, et détruit la caisse
            -etat = 3 : Si l'objet percute du feu (lorsque le joueur essaie de pousser l'objet), on le détruit
            -etat = 4 : Si l'objet percute une flaque (lorsque le joueur essaie de pousser l'objet), on fait avancer l'objet tout seul jusqu`à ce qu'il rencontre un mur, un trou, du feu...
    La fonction retourne un entier qui permettra d'indiquer ce qu'il faut faire en fonction de la valeur retourné, pour le joueur qui a percuté l'objet
        Si retourne 0 le joueur est bloqué par l'objet (car lui même bloqué par un mur) donc le joueur ne doit plus avancer dans la direction de l'objet
        Si retourne 1 l'objet n'est pas bloqué, en conséquence le joueur peut la pousser et doit pouvoir avancer dans la direction de l'objet
        Si retourne 2 on rencontre une sortie ! VICTOIRE !
    A noter que cette fonction avec beaucoup de paramètre du fait qu'elle fait appel à plusieurs autres fonctions
*/

int VerifBordureObjet(Collision BordureObjet[50], Collision *Bordure, const int xPlayer, const int yPlayer, int PositionObjet[50][2], int *NbCoordBordureObjet, int *NbCoordBordure, int LargeurHauteur[], int Position[], char Tuile[50][256], char Objet[50][256], int vitesse){
    
    //Le player fait 45*45 donc on ajoute 45 à x2 et Y2 pour représenter virtuellement les coordonnées de ce carré de player.
    int x1 = xPlayer, y1 = yPlayer, x2 = xPlayer+45, y2 = yPlayer+45;

    int val = 1;
    int etat, etat1;

    for (int i = 0; i < (*NbCoordBordureObjet)*2; i=i+2){    
        //Verification si collision en descendant l'objet
        if(y1<=BordureObjet[i].y && BordureObjet[i].y<=y2 && ((BordureObjet[i].x<=x1 && x1<=BordureObjet[i+1].x) || (BordureObjet[i].x<=x2 && x2<=BordureObjet[i+1].x))){
            if(etat = VerifBordure(Bordure, (BordureObjet[i].x), (BordureObjet[i].y+15), NbCoordBordure)){ //15 permet d'éviter de trop avancer l'objet dans un mur
                if(etat==1){ //rien ne bloque l'objet
                    PositionObjet[i/2][1]+=vitesse;
                    BordureObjet[i].y+=vitesse;
                    BordureObjet[i+1].y+=vitesse;
                }
                if(etat==2){ //L'objet rencontre un trou
                    RemplirTrou(BordureObjet[i].x,BordureObjet[i+1].y+vitesse+11,LargeurHauteur,Position,Tuile,1);  //On ajoute ici : "BordureObjet[i+1].y+vitesse+11" la vitesse qui correspond à l'endroit où est l'objet après avoir été poussé et + 11 une constante qui permet de bien se centrer sur la position de la tuile qui doit être détruit (évite de détruit une tuile à côté du trou)
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                    RemplirBordure(Position,Tuile,LargeurHauteur,Bordure,NbCoordBordure); //on actualise les nouvelles bordures de tuile (suppresion de la collision du trou qui vient d'être supprimé)
                }
                if(etat==3){ //L'objet rencontre du feu
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                }
                if(etat==4){ //Si on rencontre une flaque, on fait faire avancer la caisse juqu'à un mur, du feu ou un trou
                    while((etat1 = VerifBordure(Bordure, (BordureObjet[i].x), (BordureObjet[i].y+25),NbCoordBordure))){
                        if((etat1 != 2) && (etat1!= 3)){
                            PositionObjet[i/2][1]+=vitesse;
                            BordureObjet[i].y+=vitesse;
                            BordureObjet[i+1].y+=vitesse;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            else{
                val = 0;
            }
        }
        //Verification si collision en montant l'bjet
       if(y1<=BordureObjet[i+1].y && BordureObjet[i+1].y<=y2 && ((BordureObjet[i].x<=x1 && x1<=BordureObjet[i+1].x) || (BordureObjet[i].x<=x2 && x2<=BordureObjet[i+1].x))){
            if(etat = VerifBordure(Bordure, (BordureObjet[i].x), (BordureObjet[i].y-15), NbCoordBordure)){ //15 permet d'éviter de trop avancer l'objet dans un mur
                if(etat==1){ //rien ne bloque l'objet
                    PositionObjet[i/2][1]-=vitesse;
                    BordureObjet[i].y-=vitesse;
                    BordureObjet[i+1].y-=vitesse;
                }
                if(etat==2){ //L'objet rencontre un trou
                    RemplirTrou(BordureObjet[i].x,BordureObjet[i].y-vitesse-11,LargeurHauteur,Position,Tuile,1); //On soustrait ici : "BordureObjet[i].y-vitesse-11" la vitesse qui correspond à l'endroit où est l'objet après avoir été poussé et - 11 une constante qui permet de bien se centrer sur la position de la tuile qui doit être détruit (évite de détruit une tuile à côté du trou)
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                    RemplirBordure(Position,Tuile,LargeurHauteur,Bordure,NbCoordBordure); //on actualise les nouvelles bordures de tuile (suppresion de la collision du trou qui vient d'être supprimé)
                }
                if(etat==3){ //L'objet rencontre du feu
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                }
                if(etat==4){ //Si on rencontre une flaque, on fait faire avancer la caisse juqu'à un mur, du feu ou un trou
                    while((etat1=VerifBordure(Bordure, (BordureObjet[i].x), (BordureObjet[i].y-25),NbCoordBordure))){
                        if((etat1 != 2) && (etat1!= 3)){
                            PositionObjet[i/2][1]-=vitesse;
                            BordureObjet[i].y-=vitesse;
                            BordureObjet[i+1].y-=vitesse;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            else{
                val = 0;
            }
        }
        //Verification si collision en allant vers la gauche avec l'objet
        if(x1<=BordureObjet[i+1].x && BordureObjet[i+1].x<=x2 && ((BordureObjet[i].y<=y1 && y1<=BordureObjet[i+1].y) || (BordureObjet[i].y<=y2 && y2<=BordureObjet[i+1].y))){
            if(etat = VerifBordure(Bordure, (BordureObjet[i].x-15), (BordureObjet[i].y), NbCoordBordure)){ //15 permet d'éviter de trop avancer l'objet dans un mur
                if(etat==1){ //rien ne bloque l'objet
                    PositionObjet[i/2][0]-=vitesse;
                    BordureObjet[i].x-=vitesse;
                    BordureObjet[i+1].x-=vitesse;
                }
                if(etat==2){ //L'objet rencontre un trou
                    RemplirTrou(BordureObjet[i].x-vitesse-11,BordureObjet[i].y,LargeurHauteur,Position,Tuile,2); //On soustrait ici : "BordureObjet[i].x-vitesse-11" la vitesse qui correspond à l'endroit où est l'objet après avoir été poussé et - 11 une constante qui permet de bien se centrer sur la position de la tuile qui doit être détruit (évite de détruit une tuile à côté du trou)
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                    RemplirBordure(Position,Tuile,LargeurHauteur,Bordure,NbCoordBordure); //on actualise les nouvelles bordures de tuile (suppresion de la collision du trou qui vient d'être supprimé)

                }
                if(etat==3){ //L'objet rencontre du feu
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                }
                if(etat==4){ //Si on rencontre une flaque, on fait faire avancer la caisse juqu'à un mur, du feu ou un trou
                    while((etat1=VerifBordure(Bordure, (BordureObjet[i].x-25), (BordureObjet[i].y),NbCoordBordure))){
                        if((etat1 != 2) && (etat1!= 3)){
                            PositionObjet[i/2][0]-=vitesse;
                            BordureObjet[i].x-=vitesse;
                            BordureObjet[i+1].x-=vitesse;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            else{
                val = 0;
            }
        }
        //Verification si collision en allant vers la droite avec l'objet
       if(x1<=BordureObjet[i].x && BordureObjet[i].x<=x2 && ((BordureObjet[i].y<=y1 && y1<=BordureObjet[i+1].y) || (BordureObjet[i].y<=y2 && y2<=BordureObjet[i+1].y))){
            if(etat = VerifBordure(Bordure, (BordureObjet[i].x+15), (BordureObjet[i].y), NbCoordBordure)){ //15 permet d'éviter de trop avancer l'objet dans un mur
                if(etat==1){ //rien ne bloque l'objet
                    PositionObjet[i/2][0]+=vitesse;
                    BordureObjet[i].x+=vitesse;
                    BordureObjet[i+1].x+=vitesse;
                }
                if(etat==2){ //L'objet rencontre un trou
                    RemplirTrou(BordureObjet[i+1].x+vitesse+11,BordureObjet[i].y,LargeurHauteur,Position,Tuile,2); //On ajoute ici : "BordureObjet[i+1].x+vitesse+11" la vitesse qui correspond à l'endroit où est l'objet après avoir été poussé et + 11 une constante qui permet de bien se centrer sur la position de la tuile qui doit être détruit (évite de détruit une tuile à côté du trou)
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                    RemplirBordure(Position,Tuile,LargeurHauteur,Bordure,NbCoordBordure); //on actualise les nouvelles bordures de tuile (suppresion de la collision du trou qui vient d'être supprimé)
                }
                if(etat==3){ //L'objet rencontre du feu
                    DetruireLaCaisse(PositionObjet, BordureObjet, i);
                }
                if(etat==4){ //Si on rencontre une flaque, on fait faire avancer la caisse juqu'à un mur, du feu ou un trou
                    while((etat1=VerifBordure(Bordure, (BordureObjet[i].x+25), (BordureObjet[i].y),NbCoordBordure))){
                        if((etat1 != 2) && (etat1!= 3)){
                            PositionObjet[i/2][0]+=vitesse;
                            BordureObjet[i].x+=vitesse;
                            BordureObjet[i+1].x+=vitesse;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            else{
                val = 0;
            }
        }
    }
    return val;
}