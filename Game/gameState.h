#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdlib.h>
#include <time.h>
#include "shapes.h"

#define NUM_SHAPES 7

extern int playingGrid[10][20];
extern int scores;
extern int gameon;
extern int mult;
extern int invocations;

extern struct shapes currentshape;
extern struct shapes stored;

/* Function prototypes */
int reachedBottom(void);
void newBlock(void);
void lockPos(void);
int gameState(void);
void right(void);
void left(void);
int checkRows(void);
int gameOverCheck(void);
void gameOver(void);
void score(int rows);
void rotate(void);
void rotateBlock(int pivotx, int pivoty);
void down(void);
int isValid(void);
void store(void);
void setpos(struct shapes *shape);
void start(void);

#endif

