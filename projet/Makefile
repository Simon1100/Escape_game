
OBJ = jeu.o FinDeProgramme.o GestionDesCollisions.o GestionDuTexte.o Affichage.o ChargementEtTelechargementDuFichier.o
BIBLIO = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

prog : $(OBJ)
	gcc $(OBJ) -o jeu $(BIBLIO) 
	# Pour utiliser le programme utilisez la commande : ./jeu
jeu.o : jeu.c
	gcc -c jeu.c
FinDeProgramme.o : FinDeProgramme.c
	gcc -c FinDeProgramme.c
GestionDesCollisions.o : GestionDesCollisions.c
	gcc -c GestionDesCollisions.c
GestionDuTexte.o : GestionDuTexte.c
	gcc -c GestionDuTexte.c
Affichage.o : Affichage.c
	gcc -c Affichage.c
ChargementEtTelechargementDuFichier.o : ChargementEtTelechargementDuFichier.c
	gcc -c ChargementEtTelechargementDuFichier.c
clean :
	rm -f *.o
