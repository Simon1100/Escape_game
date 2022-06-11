//On définit une structure Collision qui va nous permettre de stocker les coordonnées des endroits où les objets et player ne peuvent pas passser
typedef struct Collision{
        int NbCollision;
        int x;
        int y;
}Collision;

int NumDecor(char*, char[][256]);

void RemplirBordure(int*,char[][256],int*, Collision*, int*);

void RemplirBordureObjet(int[][2],char[][256],int*, Collision*, int*);

int VerifBordure(Collision*, const int, const int, int*);

void RemplirTrou(int, int, int*, int*, char[][256], const int);

void DetruireLaCaisse(int[][2], Collision*, const int);

int VerifBordureObjet(Collision*,Collision*,const int, const int, int[][2], int*, int*, int*, int*, char[][256], char[][256], int);