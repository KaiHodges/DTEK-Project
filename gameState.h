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
int scores=0;
int gameon =1;
int mult = 1;
int invocations = 0;


struct shapes currentshape;
struct shapes stored;


void gameOver();
int gameOverCheck();
int reachedBottom();
void newBlock();
void lockPos();
void right();
void left();
int gameState();
int checkRows();
void score(int rows);
void rotate();  
void rotateBlock(int pivotx,int pivoty);
void down();
int isValid();
void store();
void setpos(struct shapes *shape);

void handle_interrupt(unsigned cause){
     if (cause==16){
  volatile short* volatile TO; 
  TO=(short*)0x04000020;
  *TO=0;
  invocations++;
  if(invocations==10/mult){
    invocations=0;
    down();
  
  }
//button
}if(cause==18){
    volatile int *  ptr;
    ptr=(int*)0x040000d0;
    int press = *ptr & 0xF;
    if(press==1){
        left();
    }
    if(press==2){
        store();
    }
    if(press==4){
        rotate();
    }
    if(press==8){
        right();
    }


    ptr+=3;
    int config=*ptr;
    config|=1<<0;
    *ptr=config;
  
}
    gameState();

}

/**
 * 
 * 
 */
void labinit(void)
{
  enable_interrupt();
  volatile int* volatile ptr; 
  ptr=(int*)0x4000020;
  *ptr=0;
  int time = 3000000; //30 Mhz/10 is 3 million actions to get 100 ms  
  int periodh = time>>16;
  int periodl = time & 0xffff;
  ptr+=2;
  *ptr=periodl;
  ptr+=1;
  *ptr=periodh;
  ptr-=2;
  int config = *ptr;
  config|=1<<2; // bitwise logic to set bitt <<k to one
  config|=1<<1;
  config|=1<<0;
  *ptr=config;

  //buttons
  volatile int *  ptr2;
  ptr2=(int*)0x040000d0;
  ptr2+=2;
  *ptr2=1;
  ptr2++;
  
}


/** reachedBottom
 * type: the type of block ie. L, I etc.
 * x: current x position (read each blocks center postion in there implimitation)
 * y: current y postion  ^^
 * rotation: what rotation the block has 
 * returns an in with value 0 if the block cant move and any other value otherwise
 */
int reachedBottom(){
    int y1 = currentshape.y[0] +1;
    int y2 = currentshape.y[1] +1;
    int y3 = currentshape.y[2] +1;
    int y4 = currentshape.y[3] +1;
    int x1 = currentshape.x[0];
    int x2 = currentshape.x[1];
    int x3 = currentshape.x[2];
    int x4 = currentshape.x[3];
    if(playingGrid[x1][y1]!=0||y1==20){
        return 1;
    }
    if(playingGrid[x2][y2]!=0||y2==20){
        return 1;
    }
    if(playingGrid[x3][y3]!=0||y3==20){
        return 1;
    }
    if(playingGrid[x4][y4]!=0||y4==20){
        return 1;
    }
    return 0;

}
/** newBlock
 * changes the object of current shape to the next block 
 * no inputs
 * no returns
 */
void newBlock(){
    int x = rand();
    currentshape.type=x;
    setpos(&currentshape);
}
void setpos(struct shapes *shape){
    switch ( (*shape).type)
    {
    case  0: //I block
        
        (*shape).x[0]=5;
        (*shape).x[1]=5;
        (*shape).x[2]=5;
        (*shape).x[3]=5;
        (*shape).y[0]=0;
        (*shape).y[1]=1;
        (*shape).y[2]=2;
        (*shape).y[3]=3;
        
        break;
    case  1: //O block
        
        (*shape).x[0]=5;
        (*shape).x[1]=6;
        (*shape).x[2]=5;
        (*shape).x[3]=6;
        (*shape).y[0]=0;
        (*shape).y[1]=0;
        (*shape).y[2]=1;
        (*shape).y[3]=1;
        
        break;
    case  2:// L shape
        (*shape).x[0]=5;
        (*shape).x[1]=5;
        (*shape).x[2]=5;
        (*shape).x[3]=6;
        (*shape).y[0]=0;
        (*shape).y[1]=1;
        (*shape).y[2]=2;
        (*shape).y[3]=2;
        break;
    case  3: //J shape
        (*shape).x[0]=5;
        (*shape).x[1]=5;
        (*shape).x[2]=5;
        (*shape).x[3]=4;
        (*shape).y[0]=0;
        (*shape).y[1]=1;
        (*shape).y[2]=2;
        (*shape).y[3]=2;
        break;
    case  4: //T block
        (*shape).x[0]=5;
        (*shape).x[1]=4;
        (*shape).x[2]=6;
        (*shape).x[3]=5;
        (*shape).y[0]=0;
        (*shape).y[1]=0;
        (*shape).y[2]=0;
        (*shape).y[3]=1;
        break;
    case  5: //S block
        (*shape).x[0]=5;
        (*shape).x[1]=6;
        (*shape).x[2]=5;
        (*shape).x[3]=4;
        (*shape).y[0]=0;
        (*shape).y[1]=0;
        (*shape).y[2]=1;
        (*shape).y[3]=1;
        break;
    case  6: //T block
        (*shape).x[0]=5;
        (*shape).x[1]=4;
        (*shape).x[2]=5;
        (*shape).x[3]=6;
        (*shape).y[0]=0;
        (*shape).y[1]=0;
        (*shape).y[2]=1;
        (*shape).y[3]=1;
        break;
    default:
        break;
    }
}
void lockPos(){
    int type = currentshape.type;
    playingGrid[currentshape.x[0]][currentshape.y[0]]=type;
    playingGrid[currentshape.x[1]][currentshape.y[1]]=type;
    playingGrid[currentshape.x[2]][currentshape.y[2]]=type;
    playingGrid[currentshape.x[3]][currentshape.y[3]]=type;
}

/**gameState
 * controlls the flow of the game and the grid
 */
int gameState(){
    if(reachedBottom()==1){
        lockPos();
        score(checkRows());
        if(gameOverCheck()==0){
            gameOver();
            return 0;
        }
        newBlock();
    }
    return 1;
}
/**right
 * moves the player right if it is a lega move
 * 
 */
void right(){
    struct shapes prev=currentshape;
    currentshape.x[0]++;
    currentshape.x[1]++;
    currentshape.x[2]++;
    currentshape.x[3]++;
    if(!isValid()){
        currentshape=prev;
    }
}
/**left
 * moves the user left if it is a legal move
 * no input
 * no return value
 */
void left(){
   struct shapes prev=currentshape;
    currentshape.x[0]--;
    currentshape.x[1]--;
    currentshape.x[2]--;
    currentshape.x[3]--;
    if(!isValid()){
        currentshape=prev;
    }
}

/**checkRows
 * checks if any row is filled, if so is the case delets it and send it to points
 *  returns an int wich is the number of rows cleared
 */
int checkRows(){
    int rowsCleared=0;
    //outer loop to go through each row
    for(int y=19;y>=0; y--){
        int rowfull=1;
        //iner loop to check each box in a row
        for (int x=0; x < 10; x++)
        {
          if(playingGrid[x][y]!=1){
            rowfull=0;
            break;
          }  
        }
        //if a row is full we clear it and increase the score
        if(rowfull==1){
            rowsCleared++;
            //reset row
            for (int x=0; x < 10; x++)
            {
                playingGrid[x][y]=0;
            }
            //gravity/pull down the rows above
            for (int y2 = y; y2 > 0; y2--) {
                    for (int x = 0; x < 10; x++) {
                        playingGrid[x][y2] = playingGrid[x][y2-1];
                    }
            }
            //reset top row
            for (int x = 0; x < 10; x++) {
                playingGrid[x][0] = 0;
            }
            y++;
        }
    }
    return rowsCleared;
}

/** game over check
 * if the top row has a block the game is over
 */
int gameOverCheck(){
    for (int x=0; x < 10; x++)
        {
          if(playingGrid[x][0]==1){
            return 0;
          }  
        }
        return 1;
}

void gameOver(){
gameon=0;
}

void score(int rows){
    int tot = 100*rows;
    tot*=mult;
    scores+=tot;
    mult=scores/1000;
    if(mult<1){
        mult=1;
    }
}
//
void rotate( ){
    struct shapes prev=currentshape;
    switch (currentshape.type)
    {
     case  0: //I block
       rotateBlock(currentshape.x[1],currentshape.y[1]);
        break;
    case  1: //O block
        break;
    case  2:// L shape
        rotateBlock(currentshape.x[2],currentshape.y[2]);
        break;
    case  3: //J shape
        rotateBlock(currentshape.x[3],currentshape.y[3]);
        break;
    case  4: //T block
        rotateBlock(currentshape.x[0],currentshape.y[0]);
        break;
    case  5: //S block
        rotateBlock(currentshape.x[0],currentshape.y[0]);
        break;
    case  6: //T block
        rotateBlock(currentshape.x[0],currentshape.y[0]);
        break;
    default:
        break;
    }
    if(!isValid()){
        currentshape=prev;
    }
}
/**helper function for rotate 
 * rotates using matrix multiplication on a pivot
 * (check linalg)
 */
void rotateBlock(int pivotx, int pivoty){
    int tempx;
    int tempy;

    for(int i =0;i<4;i++){
        //pos relative to pivot
        int relx = currentshape.x[i] - pivotx;
        int rely = currentshape.y[i] - pivoty;

        tempx=-rely;
        tempy=relx;

        currentshape.x[i]=pivotx+tempx;
        currentshape.y[i]=pivoty+tempy;
    }



}

void down(){
    currentshape.y[0]++;
    currentshape.y[1]++;
    currentshape.y[2]++;
    currentshape.y[3]++;
}

int isValid(){
  int blocks[4][2] = {
        {currentshape.x[0], currentshape.y[0]},
        {currentshape.x[1], currentshape.y[1]},
        {currentshape.x[2], currentshape.y[2]},
        {currentshape.x[3], currentshape.y[3]}
    };
    
    for (int i = 0; i < 4; i++) {
        int x = blocks[i][0];
        int y = blocks[i][1];
        
        // Check boundaries
        if (x < 0 || x >= 10 || y < 0 || y >= 20) {
            return 0;
        }
        
        // Check collision with placed blocks
        if (playingGrid[x][y] != 0) {
            return 0;
        }
    }
    return 1;
}

void store(){
    if(stored.type==-1){
        stored=currentshape;
        newBlock;
    }else{
    struct shapes temp;
    temp=currentshape;
    currentshape=stored;
    stored=temp; 
    setpos(&currentshape);
    }
}

void start(){
    stored.type=-1;
    labinit();
    srand(time(NULL));
    newBlock();
    playingGrid[][]={0};
    scores=0;
    gameon =1;
    mult = 1;
    invocations = 0;
}
int main(int argc, char const *argv[])
{   
    return 0;
}
