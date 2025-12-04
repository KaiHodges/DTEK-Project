#include "gameState.h"
#include <stdint.h>   // uint32_t

/**the following part is our implimentation of a random numbergenerator
 * the algorithm uses lcg, we have not reinvented the weel
 */

uint32_t seed =0;
uint32_t lcg_parkmiller(uint32_t *state);
static uint32_t rngNumber = 0;

void srand() {
    if (seed == 0) seed = 1;
    rngNumber = seed;
}
uint32_t rand(void){
    lcg_parkmiller(&rngNumber);
    return rngNumber;
}

/**recreating random numbergenerators is an idiotic task and will lead to poor generation
 * park miller uses lcg ( linear congruential generator) which uses modulo calculations
 * they build upon  D. H. Lehmer algorithm proposed in 1951 which has been the most influational
 * system. We Chose to use the already existing implimentation. The function belowe is from
 * wikipedias page about lehmer generators and with function name
 * uint32_t lcg_parkmiller(uint32_t *state)
 * link:https://en.wikipedia.org/wiki/Lehmer_random_number_generator
 * the reason we use this is since the compiler does not allow nonstandard libraries
 */
uint32_t lcg_parkmiller(uint32_t *state)
{
	// Precomputed parameters for Schrage's method
	const uint32_t M = 0x7fffffff;
	const uint32_t A = 48271;
	const uint32_t Q = M / A;    // 44488
	const uint32_t R = M % A;    //  3399

	uint32_t div = *state / Q;	// max: M / Q = A = 48,271
	uint32_t rem = *state % Q;	// max: Q - 1     = 44,487

	int32_t s = rem * A;	// max: 44,487 * 48,271 = 2,147,431,977 = 0x7fff3629
	int32_t t = div * R;	// max: 48,271 *  3,399 =   164,073,129
	int32_t result = s - t;

	if (result < 0)
		result += M;

	return *state = result;
}


/* ===== Global variable definitions ===== */

int playingGrid[10][20] = {0};
int scores = 0;
int gameon = 2;
int mult = 1;
int invocations = 0;
int storing =0;

struct shapes currentshape;
struct shapes stored;

/* ===== Function implementations ===== */

int reachedBottom(void){
    int y1 = currentshape.y[0];
    int y2 = currentshape.y[1];
    int y3 = currentshape.y[2];
    int y4 = currentshape.y[3];
    int x1 = currentshape.x[0];
    int x2 = currentshape.x[1];
    int x3 = currentshape.x[2];
    int x4 = currentshape.x[3];

    if (y1 == 19 || playingGrid[x1][y1+1] != 0) return 1;
    if (y2 == 19 || playingGrid[x2][y2+1] != 0) return 1;
    if (y3 == 19 || playingGrid[x3][y3+1] != 0) return 1;
    if (y4 == 19 || playingGrid[x4][y4+1] != 0) return 1;

    return 0;
}

void newBlock(void){
    int x = (rand() % NUM_SHAPES) + 1;
    currentshape.type = x;
    setpos(&currentshape);
    storing=0;

}

void setpos(struct shapes *shape){
    switch (shape->type) {
        case 1:  // I
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=5; shape->y[1]=1;
            shape->x[2]=5; shape->y[2]=2;
            shape->x[3]=5; shape->y[3]=3;
            break;

        case 2:  // O
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=6; shape->y[1]=0;
            shape->x[2]=5; shape->y[2]=1;
            shape->x[3]=6; shape->y[3]=1;
            break;

        case 3:  // L
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=5; shape->y[1]=1;
            shape->x[2]=5; shape->y[2]=2;
            shape->x[3]=6; shape->y[3]=2;
            break;

        case 4:  // J
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=5; shape->y[1]=1;
            shape->x[2]=5; shape->y[2]=2;
            shape->x[3]=4; shape->y[3]=2;
            break;

        case 5:  // T
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=4; shape->y[1]=0;
            shape->x[2]=6; shape->y[2]=0;
            shape->x[3]=5; shape->y[3]=1;
            break;

        case 6:  // S
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=6; shape->y[1]=0;
            shape->x[2]=5; shape->y[2]=1;
            shape->x[3]=4; shape->y[3]=1;
            break;

        case 7:  // Z
            shape->x[0]=5; shape->y[0]=0;
            shape->x[1]=4; shape->y[1]=0;
            shape->x[2]=5; shape->y[2]=1;
            shape->x[3]=6; shape->y[3]=1;
            break;
    }
}

void lockPos(void){
    int t = currentshape.type;
    for (int i = 0; i < 4; i++)
        playingGrid[currentshape.x[i]][currentshape.y[i]] = t;
}

int gameState(void){
    if (reachedBottom()) {
        lockPos();
        newBlock();
    }
    return 1;
}

void right(void){
    struct shapes prev = currentshape;
    for (int i = 0; i < 4; i++) currentshape.x[i]++;
    if (!isValid()) currentshape = prev;
}

void left(void){
    struct shapes prev = currentshape;
    for (int i = 0; i < 4; i++) currentshape.x[i]--;
    if (!isValid()) currentshape = prev;
}

int checkRows(void){
    int rowsCleared = 0;

    for (int y = 19; y >= 0; y--) {
        int emptyFound = 0;

        for (int x = 0; x < 10; x++)
            if (playingGrid[x][y] == 0){
                emptyFound = 1;
                break;
            }
        if (emptyFound==0) {
            rowsCleared++;

            for (int x = 0; x < 10; x++){
                playingGrid[x][y] = 0;
            }

            for (int y2 = y; y2 > 0; y2--){
                for (int x = 0; x < 10; x++)
                    playingGrid[x][y2] = playingGrid[x][y2 - 1];
            }

            for (int x = 0; x < 10; x++){
                playingGrid[x][0] = 0;
            }
            y++;
        }
    }

    return rowsCleared;
}

int gameOverCheck(void){
    for (int x = 0; x < 10; x++)
        if (playingGrid[x][0] != 0)
            return 0;
    return 1;
}



void score(int rows){
    int tot = rows*rows * 100 * mult;
    scores += tot;
    
    mult = scores / 1000;
    if (mult < 1) mult = 1;
}

void rotate(void){
    struct shapes prev = currentshape;

    switch (currentshape.type) {
        case 1: rotateBlock(currentshape.x[1], currentshape.y[1]); break;
        case 3: rotateBlock(currentshape.x[2], currentshape.y[2]); break;
        case 4: rotateBlock(currentshape.x[3], currentshape.y[3]); break;
        case 5: rotateBlock(currentshape.x[0], currentshape.y[0]); break;
        case 6: rotateBlock(currentshape.x[0], currentshape.y[0]); break;
        case 7: rotateBlock(currentshape.x[0], currentshape.y[0]); break;
        default: break;
    }

    if (!isValid()) currentshape = prev;
}

void rotateBlock(int px, int py){
    for (int i = 0; i < 4; i++) {
        int rx = currentshape.x[i] - px;
        int ry = currentshape.y[i] - py;
        currentshape.x[i] = px - ry;
        currentshape.y[i] = py + rx;
    }
}

void down(void){
    for (int i = 0; i < 4; i++) currentshape.y[i]++;
}

int isValid(void){
    for (int i = 0; i < 4; i++) {
        int x = currentshape.x[i];
        int y = currentshape.y[i];

        if (x < 0 || x >= 10 || y < 0 || y >= 20)
            return 0;

        if (playingGrid[x][y] != 0)
            return 0;
    }
    return 1;
}

void store(void){
    if (storing==0){
        storing=1;
        if (stored.type == -1) {
            stored = currentshape;
            newBlock();
        } else {
            struct shapes tmp = currentshape;
            currentshape = stored;
            stored = tmp;
            setpos(&currentshape);
        }
    }
}

void start(void){
    stored.type = -1;
    srand();

    newBlock();

    for (int y = 0; y < 20; y++)
        for (int x = 0; x < 10; x++)
            playingGrid[x][y] = 0;

    scores = 0;
    gameon = 1;
    mult = 1;
    invocations = 0;
}

