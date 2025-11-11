#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shapes.h"
#define RAND_MAX = 6;

//the playing grid wich is a 10 by 20 grid, where first element is row and second colum,
//ie x and y, [0][0] is the top left corner and [10][20 is bottom right corner
//here 0 symbolises empty and NOT 0 symbolizes that is filled by a block.
//potentially you can use the values to symbolise a specific color, 
//this might not be done in this implimitation depending on available time
int playingGrid[10][20]={0};



int reachedBottom();
void newBlock();
void lockPos();
int gamesstate();
struct shapes currentshape;
/** reachedBottom
 * type: the type of block ie. L, I etc.
 * x: current x position (read each blocks center postion in there implimitation)
 * y: current y postion  ^^
 * rotation: what rotation the block has 
 * returns an in with value 0 if the block cant move and any other value otherwise
 */
int reachedBottom(){
    int y1 = currentshape.y1;
    int y2 = currentshape.y2;
    int y3 = currentshape.y3;
    int y4 = currentshape.y4;
    int x1 = currentshape.x1;
    int x2 = currentshape.x2;
    int x3 = currentshape.x3;
    int x4 = currentshape.x4;
    if(playingGrid[x1][y1]==1||y1==20){
        return 1;
    }
    if(playingGrid[x2][y2]==1||y2==20){
        return 1;
    }
    if(playingGrid[x3][y3]==1||y3==20){
        return 1;
    }
    if(playingGrid[x4][y4]==1||y4==20){
        return 1;
    }
    return 0;

}
/** newBlock
 * changes the object of current shape to the next block 
 * no inputs
 * no returns
 */
void newBlcok(){
    int x = rand();
    switch (x)
    {
    case  0: //I block
        currentshape.type=0;
        currentshape.x1=5;
        currentshape.x2=5;
        currentshape.x3=5;
        currentshape.x4=5;
        currentshape.y1=0;
        currentshape.y2=1;
        currentshape.y3=2;
        currentshape.y4=3;
        
        break;
    case  1: //O block
        currentshape.type=1;
        currentshape.x1=5;
        currentshape.x2=6;
        currentshape.x3=5;
        currentshape.x4=6;
        currentshape.y1=0;
        currentshape.y2=0;
        currentshape.y3=1;
        currentshape.y4=1;
        
        break;
    case  2:// L shape
        currentshape.type=2;
        currentshape.x1=5;
        currentshape.x2=5;
        currentshape.x3=5;
        currentshape.x4=6;
        currentshape.y1=0;
        currentshape.y2=1;
        currentshape.y3=2;
        currentshape.y4=2;
        break;
    case  3: //J shape
        currentshape.type=3;
        currentshape.x1=5;
        currentshape.x2=5;
        currentshape.x3=5;
        currentshape.x4=4;
        currentshape.y1=0;
        currentshape.y2=1;
        currentshape.y3=2;
        currentshape.y4=2;
        break;
    case  4: //T block
        currentshape.type=4;
        currentshape.x1=5;
        currentshape.x2=4;
        currentshape.x3=6;
        currentshape.x4=5;
        currentshape.y1=0;
        currentshape.y2=0;
        currentshape.y3=0;
        currentshape.y4=1;
        break;
    case  5: //S block
        currentshape.type=5;
        currentshape.x1=5;
        currentshape.x2=6;
        currentshape.x3=5;
        currentshape.x4=4;
        currentshape.y1=0;
        currentshape.y2=0;
        currentshape.y3=1;
        currentshape.y4=1;
        break;
    case  6: //T block
        currentshape.type=6;
        currentshape.x1=5;
        currentshape.x2=4;
        currentshape.x3=5;
        currentshape.x4=6;
        currentshape.y1=0;
        currentshape.y2=0;
        currentshape.y3=1;
        currentshape.y4=1;
        break;
    default:
        break;
    }
}
void lockPos(){
    playingGrid[currentshape.x1][currentshape.y1]=1;
    playingGrid[currentshape.x2][currentshape.y2]=1;
    playingGrid[currentshape.x3][currentshape.y3]=1;
    playingGrid[currentshape.x4][currentshape.y4]=1;
}
int gameState(){

}
void right(){
    int y1 = currentshape.y1;
    int y2 = currentshape.y2;
    int y3 = currentshape.y3;
    int y4 = currentshape.y4;
    int x1 = currentshape.x1+1;
    int x2 = currentshape.x2+1;
    int x3 = currentshape.x3+1;
    int x4 = currentshape.x4+1;
     if(playingGrid[x1][y1]==1||x1==10){
        return;
    }
    if(playingGrid[x2][y2]==1||x2==10){
        return;
    }
    if(playingGrid[x3][y3]==1||x3==10){
        return;
    }
    if(playingGrid[x4][y4]==1||x4==10){
        return;
    }
    currentshape.x1=x1;
    currentshape.x2=x2;
    currentshape.x3=x3;
    currentshape.x4=x4;
}
void left(){
    int y1 = currentshape.y1;
    int y2 = currentshape.y2;
    int y3 = currentshape.y3;
    int y4 = currentshape.y4;
    int x1 = currentshape.x1-1;
    int x2 = currentshape.x2-1;
    int x3 = currentshape.x3-1;
    int x4 = currentshape.x4-1;
    //check that the move is legal, both that the position is not ocupied
    //and that the next value is inside the grid
    if(playingGrid[x1][y1]==1||x1==-1){
        return;
    }
    if(playingGrid[x2][y2]==1||x2==-1){
        return;
    }
    if(playingGrid[x3][y3]==1||x3==-1){
        return;
    }
    if(playingGrid[x4][y4]==1||x4==-1){
        return;
    }
    currentshape.x1=x1;
    currentshape.x2=x2;
    currentshape.x3=x3;
    currentshape.x4=x4;
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    return 0;
}
