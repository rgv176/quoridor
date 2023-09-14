

#include "globals.h"                                       // Toutes les sous-fonctions utilisés dans le quoridor.c

void InitBoard(cell board[XDIM][YDIM]);
int ValidFenceSpot(cell boardOrig[XDIM][YDIM], int iColor, int x, int y, char cSide);
void AddFence(cell board[XDIM][YDIM], int iColor, int x, int y, char cSide);
void ShowBoard(cell board[XDIM][YDIM]);
int RecursivePathCheck(cell board[XDIM][YDIM], int board_visited[XDIM][YDIM], int x, int y, int desty, char cDir);
int PathExists(cell board[XDIM][YDIM],int x, int y, int desty);
int IsValidMove(cell board[XDIM][YDIM], char *sMove, int iWhoseMove);
int FencePathNecessary(cell board[XDIM][YDIM], int x, int y,char sMove);
int MakeMove(cell board[XDIM][YDIM], char *sMove, int iWhoseMove);
int CheckForWin();
void MoveGen(cell board[XDIM][YDIM], int iWhoseMove, char mlist[150][7], int p);
int astar(cell board[XDIM][YDIM], int x, int y, int desty);
int heuristic(cell board[XDIM][YDIM], int imycolor);
int maxvalue(cell board[XDIM][YDIM], int iComp, int iWhose, int alpha, int beta, int depth, char *b);
int minvalue(cell board[XDIM][YDIM], int iComp, int iWhose, int alpha, int beta, int depth, char *b);


