

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "globals.h"
#include "quoridor.h"


main()
{
 int i,finjeu;
 char s[100];                             //declaration d'une chaine provisoire pour le nom des joueurs
 int ajouer;                              // entier pour le comparer par la suite pour l'affichage des donnees du joueurs
 int desty;
 char mlist[150][7];
 char best[10];
 int joueur1,joueur2;
 char Njoueur1[20];                           //nom du joueur 1
 char Njoueur2[20];                           //nom du joueur 2
 FILE *f;



 iXNeighborhood=2;
 iYNeighborhood=1;
 iMoveNum=1;
 srand(getpid());

 printf("Nom joueur 1: ");
 gets(s);
 sscanf(s,"%s",&Njoueur1);
 joueur1=1;
 printf("Nom joueur 2: ");
 gets(s);
 sscanf(s,"%s",&Njoueur2);
 joueur2=2;
 printf("\n\n");
 finjeu=0;
 InitBoard(board);
 ajouer=joueur1;
 while (!finjeu)                                                           //tant qu'il n'y a pas de vainqueur
 {
 	ShowBoard(board);

	if (ajouer==joueur1) printf("Symbol %d:@ du joueur 1 : %s\n",iMoveNum,Njoueur1);  //on compare la valeure de "ajouter" avec celle de "joueur 1" pour afficher les informations
  	else printf("Symbol %d:# du joueur 2 : %s\n ",iMoveNum,Njoueur2);                 // sur le joueur concerne
	printf(" u,d,l,r pour bouger,\n X Y r/b pour un mur,\n n si aucune action:\n");   //Affichage du choix qui s'offre au joueur pour la partie
	gets(s);

	if (IsValidMove(board,s,ajouer))                                              // appel du sous-programme "IsValidMove" pour determiner la possibilite du deplacement
	{                                                                             // et donc l'effectuer
		if ((s[0]=='u') || (s[0]=='d') || (s[0]=='l') || (s[0]=='r')) {
			i_aryLastWasMove[ajouer]=1;
		}
		else {
			i_aryLastWasMove[ajouer]=0;
		}
		i_aryLastXLoc[ajouer]=i_aryXLoc[ajouer];
		i_aryLastYLoc[ajouer]=i_aryYLoc[ajouer];
		MakeMove(board,s,ajouer);                                                  //appel du sous-programme "MakeMove" pour avancer les pions
		if (ajouer==joueur1) ajouer=joueur2;
		else ajouer=joueur1;
		iMoveNum++;                                                               //incrementation du nombre lorsqu'il s'agit de notre tour
	}
	else if (s[0]=='n')
	{
		if (ajouer==joueur1) ajouer=joueur2;
		else ajouer=joueur1;
		iMoveNum++;                                                                //incrementation du nombre lorsqu'il s'agit de notre tour
	}
	else
	{
		printf("\n Mouvement non valide    Try again.\n");
	}
	i=CheckForWin();
	if (i!=EMPTY)			/* quelqu'un a gagne */
	{
		finjeu=1;
	 	ShowBoard(board);
		printf("\n\nLa partie est termine!  ");
		if (i==WHITE) printf("\nWhite (@) a gagne!\n");
		else if (i==BLACK) printf("\nBlack (#) a gagne!\n");

	}
 }
}

