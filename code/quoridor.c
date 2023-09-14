

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "globals.h"
#include "quoridor.h"



// Initialisation pour nouvelle jeu.

void InitBoard(cell board[XDIM][YDIM])          //dimensions ont ete definis dans le quoridor.h  -> 9x9
{
 int i,j;

 i_aryFences[WHITE]=NUMFENCES;        //on initialise le nombre de barrieres de chaque joueur(WHITE, BLACK) a NUMFENCES=10
 i_aryFences[BLACK]=NUMFENCES;
 for (i=0; i<XDIM; i++)				/* On vide le plateau */
	for (j=0; j<YDIM; j++)
	{
  		board[i][j].state=EMPTY;          // On initialise l'etat de toutes les cases a EMPTY=0 pour les barrieres
  		board[i][j].r=OPEN;               // On initialise l'etat pour lequel les cases ne sont pas consideres comme vide
  		board[i][j].b=OPEN;                // c'est-a-dire si on place une barriere horizontalement ou verticalement
  		board[i][j].rvalid=VALID;          //On fait la meme chose pour rvalid et bvalid = validation finale de la pose des barrieres
  		board[i][j].bvalid=VALID;
  		board[i][j].fsource=OPEN;
	}
 board[4][0].state=BLACK;			           /* On place le pion noir en haut*/
 board[4][8].state=WHITE;			           /* On place le pion blanc en bas */
 i_aryXLoc[WHITE]=4;                           //On initialise la position des pions pour chaque joueurs
 i_aryXLoc[BLACK]=4;                           // au milieu et opposés au noveau des bords du plateau
 i_aryYLoc[WHITE]=8;
 i_aryYLoc[BLACK]=0;
 i_aryLastXLoc[WHITE]=4;                        //on initialise les coordonnees de la derniere position avec les memes coordonnes du dessus
 i_aryLastXLoc[BLACK]=4;
 i_aryLastYLoc[WHITE]=8;
 i_aryLastYLoc[BLACK]=0;
 i_aryLastWasMove[WHITE]=0;                      //On initialise le dernier mouvement d'un pion a 0 au debut de la partie
 i_aryLastWasMove[BLACK]=0;
}


// tester si l'obstacle est valide
int ValidFenceSpot(cell boardOrig[XDIM][YDIM], int iColor, int x, int y, char cSide)    // on verifie les conditions du plateau pour placer les barrieres
{
 cell board[XDIM][YDIM];

 /* On va copier a un plateau temporaire*/
 memcpy((char *) &board[0], (char *) &boardOrig[0], XDIM*YDIM*sizeof(cell));

 if ((x<0) || (x>=XDIM-1)) return(0);                                                  // si les dimensions du plateau ne sont pas correctes, on retourne 0
 if ((y<0) || (y>=YDIM-1)) return(0);
 if ((cSide!='b') && (cSide!='r')) return(0);                                       // si on ne place la barriere ni horizontalement, ni verticalement, on retourne 0
 if (cSide=='b') 	                                                            	/* valide de mettre la barriere en bas? */
 {
	if ((board[x][y].b==BLOCKED) || (board[x+1][y].b==BLOCKED)) return(0);          // si la place est deja occupe par un obstacle on retourne 0
	if (board[x][y].bvalid==INVALID) return(0);
   	if (x>0)
 	{
		board[x-1][y].bvalid=INVALID;
	}
	board[x][y].rvalid=INVALID;                                                      // on bloque la place pour ne pas mettre de barriere verticalement
	board[x][y].bvalid=INVALID;                                                      //on bloque la place pour ne pas mettre de barriere horizontale
	board[x][y].b=BLOCKED;                                                          //on bloque la place pour ne plus replacer une barriere a la suite du jeu
	board[x+1][y].b=BLOCKED;                                                        // on bloque aussi la position en x+1 car c'est une barriere horizontale
	board[x][y].fsource=BOTTOM;                                                     // on trace la barriere en bas
	if (i_aryFences[WHITE]+i_aryFences[BLACK] > 19) return(1);
	/* est ce que ca bloque l'obstacle au but */
	if (FencePathNecessary(board,x,y,cSide)) {
 	  if ((PathExists(board, i_aryXLoc[WHITE], i_aryYLoc[WHITE], 0)) &&                    //appel de la sous-fonction "PathExists" pour verifier qu'il existe toujours
	      (PathExists(board, i_aryXLoc[BLACK], i_aryYLoc[BLACK], YDIM-1)))                  // un chemin pour le pion apres le placement de la barriere
	  {
		return(1);
	  }
        }
	else {
		return(1);
	}
 }
 else if (cSide=='r') 	                                                                  	/* valide de mettre obstacle àdroite? */
 {
	if ((board[x][y].r==BLOCKED) || (board[x][y+1].r==BLOCKED)) return(0);                     //on effectue la meme demarche qu'en haut pour une barriere verticale
	if (board[x][y].rvalid==INVALID) return(0);
   	if (y>0)
 	{
		board[x][y-1].rvalid=INVALID;
	}
	board[x][y].rvalid=INVALID;
	board[x][y].bvalid=INVALID;
	board[x][y].r=BLOCKED;
	board[x][y+1].r=BLOCKED;
	board[x][y].fsource=RIGHT;                                          // on trace la barriere a droite

	                    /* est ce que ca bloque l'obstacle au but  */
	if (i_aryFences[WHITE]+i_aryFences[BLACK] > 19) return(1);
	if (FencePathNecessary(board,x,y,cSide)) {
 	  if ((PathExists(board, i_aryXLoc[WHITE], i_aryYLoc[WHITE], 0)) &&
	      (PathExists(board, i_aryXLoc[BLACK], i_aryYLoc[BLACK], YDIM-1)))
	  {
		return(1);
	  }
	}
	else {
		return(1);
	}
 }
 return(0);
}

/*** voir si c'est necaissaire verifier itiniraire avane de faire appel a un autre sous programme:  "PathExists" ***/


int FencePathNecessary(cell board[XDIM][YDIM], int x, int y, char side)
{
 if (side=='r')                                                     //si on veut placer une barriere verticale
 {
	if (y>0) {
		if (board[x][y-1].b==BLOCKED) return(1);                   // si a la position Y-1, il y a deja un obstacle, on retourne 1
		if (board[x][y-1].r==BLOCKED) return(1);
		if (x<XDIM-1) {
			if (board[x+1][y-1].b==BLOCKED) return(1);
		}
	}
	if (board[x][y].b==BLOCKED) return(1);
	if (y<YDIM-1) {
		if (board[x][y+1].b==BLOCKED) return(1);
		if (x<XDIM-1) {
			if (board[x+1][y+1].b==BLOCKED) return(1);
		}
	}
	if (x<XDIM-1) {
		if (board[x+1][y].b==BLOCKED) return(1);
	}
	if (y<YDIM-2) {
		if (board[x][y+2].r==BLOCKED) return(1);
	}
	return(0);
 }
 else
 {
 	if (x>0) {
		if (board[x-1][y].r==BLOCKED) return(1);            // on recherche la meme chose en X-1
		if (board[x-1][y].b==BLOCKED) return(1);
		if (y<YDIM-1) {
			if (board[x-1][y+1].r==BLOCKED) return(1);
		}
	}
	if (board[x][y].r==BLOCKED) return(1);
	if (y<YDIM-1) {
		if (board[x][y+1].r==BLOCKED) return(1);
	}
	if (x<XDIM-1) {
		if (board[x+1][y].r==BLOCKED) return(1);
		if (y<YDIM-1) {
			if (board[x+1][y+1].r==BLOCKED) return(1);
		}
	}
	if (x<XDIM-2) {
		if (board[x+2][y].b==BLOCKED) return(1);
	}
	return(0);
 }
 return(1);
}

/*****************************************************************
ajouter un obtacle a une position precise
 *****************************************************************/


void AddFence(cell board[XDIM][YDIM], int iColor, int x, int y, char cSide)      //Sous-programme pour placer une barriere avec en parametre le joueur, les coordonnees,
{                                                                                // et s'il s'agit d'une barriere verticale ou horizontale
 if (cSide=='b')                   // si on place la barriere horizontalement
 {
   	if (x>0)
 	{
		board[x-1][y].bvalid=INVALID;
	}
	board[x][y].rvalid=INVALID;                         //on fait passer notre case en invalide a cause de la barriere
	board[x][y].bvalid=INVALID;
	board[x][y].b=BLOCKED;
	board[x+1][y].b=BLOCKED;                            // on declare la case a droite bloque pour tracer la barriere
	board[x][y].fsource=BOTTOM;                         // on trace la barriere en bas
	i_aryFences[iColor]-=1;
 }
 else if (cSide=='r')
 {
   	if (y>0)
 	{
		board[x][y-1].rvalid=INVALID;                      //meme fonctionnement pour la barriere verticale
	}
	board[x][y].rvalid=INVALID;
	board[x][y].bvalid=INVALID;
	board[x][y].r=BLOCKED;
	board[x][y+1].r=BLOCKED;
	board[x][y].fsource=RIGHT;
	i_aryFences[iColor]-=1;
 }
}


/*****************************************************************
 montrer le plateau du jeu
 *****************************************************************/


void ShowBoard(cell board[XDIM][YDIM])                               //affichage du plateau
{
 int i,j;

 printf("\n");
 printf("   ");
 /*for (i=0; i<XDIM; i++)
 {
  printf("%d ",i);
 */
 for (i=0; i<XDIM; i++)                     //On affiche les coordonnees de l''abscisse
		printf("%c ", i+'A');
 printf("\n");                               //sous forme de plateau
 for (j=0; j<YDIM; j++)                     //on affiche les coordonnes de l'ordonne
 {
	printf("%2d ",j+1);
	for (i=0; i<XDIM; i++)
	{
		if (board[i][j].state==EMPTY) printf(".");            //on remplit les espaces vides par des "."
		else if (board[i][j].state==WHITE) printf("@");        //symbole pour un joueur
		else if (board[i][j].state==BLACK) printf("#");        // symbole pour un autre
		if (i<XDIM-1)
		{
			if (board[i][j].r==OPEN) printf(" ");              //mise en page pour l'affichage des barrieres
			else if (board[i][j].r==BLOCKED) printf("|");
		}
	}
	printf("\n   ");
	if (j<YDIM-1)
	{
		for (i=0; i<XDIM; i++)
		{
			if (board[i][j].fsource==OPEN)
			{
				if (board[i][j].b==OPEN) printf("  ");
				else if (board[i][j].b==BLOCKED) printf("-");
			}
			else if (board[i][j].fsource==BOTTOM)
			{
				printf("--");
			}
			else if (board[i][j].fsource==RIGHT)
			{
				if (board[i][j].b==BLOCKED) printf("-|");
				else printf(" |");
			}
		}
	}
	 printf("\n");
 }
 printf("joueur 1 obstacles (@): %d    joueur2 obstacles (#): %d\n",i_aryFences[WHITE], i_aryFences[BLACK]);    //affichage du nombre de barrieres restants pour chaque joueur
}


/*****************************************************************
test du chemin recursive
 *****************************************************************/

int RecursivePathCheck(cell board[XDIM][YDIM], int board_visited[XDIM][YDIM], int x, int y, int desty, char cDir) // si on ne peut aller ni a gauche, ni a droite
{
 int iCounter=0;          // appel de la fonction récursive pour verifier si on peut aller a l'une des 3 cases de l'ou on est actuellement
 int iResult;             // si on peut bouger on va rappeler la meme fonction a la cas ou on a pu bouger
 char cNewDir;            //si on ne peut plus bouger dans les 3 cellules de la haut-> plus de chemin vers la destination finale

 /* si c'est finit */
 if (y==desty) return 1;

 /* mettre lee noeud actuel comme verifier */
 board_visited[x][y]=TRUE;
 cNewDir=cDir;

 /* commencer à verifier les voisins avec cNewDir */
 while (iCounter<4)
 {
	if (cNewDir=='t')
	{
		if ((y>0) && (board_visited[x][y-1]==FALSE) && (board[x][y-1].b==OPEN))
		{
			iResult=RecursivePathCheck(board,board_visited,x,y-1,desty,cDir);
			if (iResult>0) return(iResult);
		}
		cNewDir='r';
		iCounter++;
	}
	else if (cNewDir=='r')
	{
		if ((x<XDIM-1) && (board_visited[x+1][y]==FALSE) && (board[x][y].r==OPEN))
		{
			iResult=RecursivePathCheck(board,board_visited,x+1,y,desty,cDir);
			if (iResult>0) return(iResult);
		}
		cNewDir='b';
		iCounter++;
	}
	else if (cNewDir=='b')
	{
		if ((y<YDIM-1) && (board_visited[x][y+1]==FALSE) && (board[x][y].b==OPEN))
		{
			iResult=RecursivePathCheck(board,board_visited,x,y+1,desty,cDir);
			if (iResult>0) return(iResult);
		}
		cNewDir='l';
		iCounter++;
	}
	else if (cNewDir=='l')
	{
		if ((x>0) && (board_visited[x-1][y]==FALSE) && (board[x-1][y].r==OPEN))
		{
			iResult=RecursivePathCheck(board,board_visited,x-1,y,desty,cDir);
			if (iResult>0) return(iResult);
		}
		cNewDir='t';
		iCounter++;
	}
 }
 return(0);
}


/*****************************************************************
 renvoie 1 si il existe un chemin et 0 si il y a pas de chemin
 *****************************************************************/

int PathExists(cell board[XDIM][YDIM],int x, int y, int desty)
{
 int board_visited[XDIM][YDIM];
 int i,j;

 if ((y==0) && (desty==0)) return 1;
 if ((y==YDIM-1) && (desty==YDIM-1)) return 1;

 /* Garder en memoire les endroits deja verifies*/
 for (i=0; i<XDIM; i++)
 {
	for (j=0; j<YDIM; j++)
	{
		board_visited[i][j]=FALSE;
	}
 }
 if (desty==0) i=RecursivePathCheck(board, board_visited, x, y, desty, 't');
 else i=RecursivePathCheck(board, board_visited, x, y, desty, 'b');
 return i;
}




/*****************************************************************
 renvoie 1 si le mvt est valide 0 sinon
 *
 * le deplcament : 'u','d','l','r' pour se deplacer
 * or   'x y r' to add a fence to the right/down of cell x,y
 * or   'x y b' to add a fence to the bottom/right of cell x,y
 *
 *****************************************************************/
int IsValidMove(cell board[XDIM][YDIM], char *sMove, int iWhoseMove)
{
 int x,y;
 char cDirection;
 int iOpponent;

 if (iWhoseMove==WHITE) iOpponent=BLACK;                        //on definit le joueur qui va bouger
 else iOpponent=WHITE;

 if (sMove[0]=='u')                                         // si on deplace le pion en haut
 {
	x=i_aryXLoc[iWhoseMove];                               //on verifie que la case du haut n'est pas ocuupe avec le meme fonctionnement que les barrieres
	y=i_aryYLoc[iWhoseMove];
	if (y==0) return(0);
	if (board[x][y-1].b==BLOCKED) return(0);
	if ( (y==1) &&
	     (x==i_aryXLoc[iOpponent]) &&
	     (y-1==i_aryYLoc[iOpponent]) ) return(0);
	if ( (y>1) &&
	     (x==i_aryXLoc[iOpponent]) &&
	     (y-1==i_aryYLoc[iOpponent]) &&
	     (board[x][y-2].b==BLOCKED)) return(0);
	return(1);
 }
 else if (sMove[0]=='d')                                            //on effectue la meme chose pour chaque deplacement
 {
	x=i_aryXLoc[iWhoseMove];
	y=i_aryYLoc[iWhoseMove];
	if (y==YDIM-1) return(0);
	if (board[x][y].b==BLOCKED) return(0);
	if ( (y==YDIM-2) &&
	     (x==i_aryXLoc[iOpponent]) &&
	     (y+1==i_aryYLoc[iOpponent]) ) return(0);
	if ( (y<YDIM-2) &&
	     (x==i_aryXLoc[iOpponent]) &&
	     (y+1==i_aryYLoc[iOpponent]) &&
	     (board[x][y+1].b==BLOCKED)) return(0);
	return(1);
 }
 else if (sMove[0]=='l')
 {
	x=i_aryXLoc[iWhoseMove];
	y=i_aryYLoc[iWhoseMove];
	if (x==0) return(0);
	if (board[x-1][y].r==BLOCKED) return(0);
	if ( (x==1) &&
	     (y==i_aryYLoc[iOpponent]) &&
	     (x-1==i_aryXLoc[iOpponent]) ) return(0);
	if ( (x>1) &&
	     (y==i_aryYLoc[iOpponent]) &&
	     (x-1==i_aryXLoc[iOpponent]) &&
	     (board[x-2][y].r==BLOCKED)) return(0);
	return(1);
 }
 else if (sMove[0]=='r')
 {
	x=i_aryXLoc[iWhoseMove];
	y=i_aryYLoc[iWhoseMove];
	if (x==XDIM-1) return(0);
	if (board[x][y].r==BLOCKED) return(0);
	if ( (x==XDIM-2) &&
	     (y==i_aryYLoc[iOpponent]) &&
	     (x+1==i_aryXLoc[iOpponent]) ) return(0);
	if ( (x<XDIM-2) &&
	     (y==i_aryYLoc[iOpponent]) &&
	     (x+1==i_aryXLoc[iOpponent]) &&
	     (board[x+1][y].r==BLOCKED)) return(0);
	return(1);
 }
 else 	                                              //si on choisit de mettre une barriere
 {
	if (i_aryFences[iWhoseMove]==0)
	{
		return(0);
	}
	sscanf(sMove,"%d %d %c",&x,&y,&cDirection);            //on rentre les coordonnes dans lesquelles placer la barriere
	if (ValidFenceSpot(board, iWhoseMove, x, y, cDirection)==1)       //on verifie qu'on peut la placer avec la sous fonction
	{
		return(1);
	}
 }
 return(0);
}


int MakeMove(cell board[XDIM][YDIM], char *sMove, int iWhoseMove)                //deplacement du pion apres verification
{
 int x,y;
 char cDirection;
 int iOpponent;

 if (iWhoseMove==WHITE) iOpponent=BLACK;                               //on defini le joueur qui joue
 else iOpponent=WHITE;

 if (IsValidMove(board,sMove,iWhoseMove))                            //si le mouvement qu'il effectue est valide
 {
	x=i_aryXLoc[iWhoseMove];                                         //on attribut de nouvelles coordonnees au pion
	y=i_aryYLoc[iWhoseMove];

	if (sMove[0]=='u')
	{
		board[x][y].state=EMPTY;
		if ((x==i_aryXLoc[iOpponent]) && (y-1==i_aryYLoc[iOpponent])) y-=2;	         /* on saute par dessus l'adversaire*/
		else y--;
		board[x][y].state=iWhoseMove;
		i_aryYLoc[iWhoseMove]=y;
	}
	else if (sMove[0]=='d')
	{
		board[x][y].state=EMPTY;
		if ((x==i_aryXLoc[iOpponent]) && (y+1==i_aryYLoc[iOpponent])) y+=2;            	/* on saute par dessus l'adversaire */
		else y++;
		board[x][y].state=iWhoseMove;
		i_aryYLoc[iWhoseMove]=y;
	}
	else if (sMove[0]=='l')
	{
		board[x][y].state=EMPTY;
		if ((x-1==i_aryXLoc[iOpponent]) && (y==i_aryYLoc[iOpponent])) x-=2;	                /* on saute par dessus l'adversaire */
		else x--;
		board[x][y].state=iWhoseMove;
		i_aryXLoc[iWhoseMove]=x;
	}
	else if (sMove[0]=='r')
	{
		board[x][y].state=EMPTY;
		if ((x+1==i_aryXLoc[iOpponent]) && (y==i_aryYLoc[iOpponent])) x+=2;                  	/*on saute par dessus l'adversaire*/
		else x++;
		board[x][y].state=iWhoseMove;
		i_aryXLoc[iWhoseMove]=x;
	}
	else
	{
	 sscanf(sMove,"%d %d %c",&x,&y,&cDirection);                                          //on donne les coordonnees pour placer la barriere
	 AddFence(board,iWhoseMove,x,y,cDirection);
	}
 }
 else
 {
 	return(0);
 }
}


int CheckForWin()                //On verifie le gagnant
{
 if (i_aryYLoc[WHITE]==0)        // si le joueur "WHITE" est arrive a l'aure bout
 {
	return(WHITE);                 //On retourne white
 }
 else if (i_aryYLoc[BLACK]==8)      //Si le joueur "BLACK" est arrive a l'autre bout du plateau
 {
	return(BLACK);                  //on retourne BLACK
 }
 else
 {
	return(EMPTY);                   //Sinon on retourne EMPTY      ou(EMPTY, WHITE, BLACH -> l'etat de la partie)
 }
}

void MoveGen(cell board[XDIM][YDIM], int iWhoseMove, char mlist[150][7], int p)
{
 int i,j,iMoveCount=0;
 int xdist1,ydist1,xdist2,ydist2;
 char s[50];

 /*On verifie en haut pour le blanc et en bas pour le noir*/

 if (iWhoseMove==WHITE)
 {
  if (IsValidMove(board,"u",iWhoseMove))
  {
   strcpy(mlist[iMoveCount],"u");
   iMoveCount++;
  }
  if (IsValidMove(board,"d",iWhoseMove))
  {
   strcpy(mlist[iMoveCount],"d");
   iMoveCount++;
  }
 }
 else
 {
  if (IsValidMove(board,"d",iWhoseMove))
  {
   strcpy(mlist[iMoveCount],"d");
   iMoveCount++;
  }
  if (IsValidMove(board,"u",iWhoseMove))
  {
   strcpy(mlist[iMoveCount],"u");
   iMoveCount++;
  }
 }
 if (IsValidMove(board,"r",iWhoseMove))
 {
   strcpy(mlist[iMoveCount],"r");
   iMoveCount++;
 }
 if (IsValidMove(board,"l",iWhoseMove))
 {
   strcpy(mlist[iMoveCount],"l");
   iMoveCount++;
 }

 if (i_aryFences[iWhoseMove]>0)
 {
  for (i=0; i<XDIM-1; i++)
   for (j=0; j<YDIM-1; j++)
   {
	if (p==1) xdist1=1;
	else {
 		if (i>i_aryXLoc[WHITE]) xdist1=i-i_aryXLoc[WHITE];
			else xdist1=i_aryXLoc[WHITE]-i;
		if (i>i_aryXLoc[BLACK]) xdist2=i-i_aryXLoc[BLACK];
		else xdist2=i_aryXLoc[BLACK]-i;
		if (j>i_aryYLoc[WHITE]) ydist1=j-i_aryYLoc[WHITE];
		else ydist1=i_aryYLoc[WHITE]-j;
		if (j>i_aryYLoc[BLACK]) ydist2=j-i_aryYLoc[BLACK];
		else ydist2=i_aryYLoc[BLACK]-j;
	}
        if (((xdist1<=iXNeighborhood) && (ydist1<=iYNeighborhood)) || ((xdist2<=iXNeighborhood) && (ydist2<=iYNeighborhood)))
 	{
	  sprintf(s,"%d %d r",i,j);
 	  if (IsValidMove(board,s,iWhoseMove))
	  {
	   	sprintf(s,"%d %d r",i,j);
		strcpy(mlist[iMoveCount],s);
	  	iMoveCount++;
          }
	  sprintf(s,"%d %d b",i,j);
	  if (IsValidMove(board,s,iWhoseMove))
	  {
	   	sprintf(s,"%d %d b",i,j);
		strcpy(mlist[iMoveCount],s);
	  	iMoveCount++;
          }
 	}
   }
 }
 strcpy(mlist[iMoveCount],"null");  /* fin de la liste */
}


