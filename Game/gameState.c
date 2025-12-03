#include "gameState.h"
#include <stdint.h>   // for uint32_t

/* ---- Minimal RNG + time stubs (because we're not linking full libc) ---- */

static uint32_t rng_state = 1;

void srand(unsigned int seed) {
    if (seed == 0) seed = 1;
    rng_state = seed;
}

int rand(void) {
    // simple linear congruential generator
    rng_state = rng_state * 1103515245u + 12345u;
    return (rng_state >> 16) & 0x7FFF;  // value in [0, 32767]
}

time_t time(time_t *t) {
    // fake monotonically increasing time
    static time_t fake = 1;
    if (t) *t = fake;
    return fake++;
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
    int tot = rows * 100 * mult;
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
    srand(time(NULL));

    newBlock();

    for (int y = 0; y < 20; y++)
        for (int x = 0; x < 10; x++)
            playingGrid[x][y] = 0;

    scores = 0;
    gameon = 1;
    mult = 1;
    invocations = 0;
}

