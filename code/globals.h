

#ifndef _globals_h
#define _globals_h

#define DEBUG 0
#define RANDOMIT 0
#define EMPTY 0
#define joueur_1 1
#define joueur_2 2
#define WHITE 1
#define BLACK 2
#define OPEN 1
#define BLOCKED 2
#define INVALID 0
#define VALID 1
#define RIGHT 2
#define BOTTOM 3
#define TRUE 1
#define FALSE 0

#define XDIM 9				    /* taille du plateau */
#define YDIM 9
#define NUMFENCES 10			/* nombre de barrieres par joueur */

/******************************
 Data structure for the board.
*******************************/
typedef struct cellstruct
{
 int r,b;			         /* pour placer la barriere:  right= Verticalement / bottom= Horizontalement */
 int rvalid, bvalid;		/* mettre l'obtscale valide dans right/bottom */
 int state;			       /* empty, white, or black */
 int fsource;			   /* pour l'affichage */
} cell;


/******************************
 Variables globales pour les
 obstacles, combien de barrieres
 chaque joueur a, et la
 position actuelle de chacun des
 pièces des joueurs
*******************************/


cell board[XDIM][YDIM];              // indice de 3 pour un jeu ou on a 4 joueurs, mais ici on aurait pu mettre l'indice 1 car on se limite a 2 joueurs
int i_aryFences[3];                  //tableau de tout les obstacles qui existent sur le plateau
int i_aryXLoc[3];                    //position de chaque pion suivant l'abscisse x
int i_aryYLoc[3];                    //position de chaque pion suivant l'ordonne y
int i_aryLastXLoc[3];                // Position de l'abscisse du dernier pion
int i_aryLastYLoc[3];                // Position de l'ordonne du dernier pion
int i_aryLastWasMove[3];             // dernier mouvement effectue(en haut, en bas, a droite, a gauche)
int iDepth;
int iMoveNum;
int iXNeighborhood, iYNeighborhood;

#endif
