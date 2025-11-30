#include "vga.h"
#include "gameState.h"
#include "shapes.h"   // for struct shapes

int playingGrid[10][20] = {0};
int scores = 0;
int gameon = 1;
int mult = 1;
int invocations = 0;

struct shapes currentshape;
struct shapes stored;

extern void enable_interrupt(void); // needs to be declared for the compiler

void newGame(void){
	start();
}

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
	}else if(cause==17){
		volatile int *ptr = (int*)0x04000010;
		ptr+=3;
		int switchState = *ptr;  
		
		
		if(switchState & 0x1) {       
			right();
		}
		if(switchState & 0x2) {       
			rotate();
		}
		if(switchState & 0x100) {     
			store();
		}
		if(switchState & 0x200) {     
			left();
		}
		
		// Clear interrupt
		ptr += 3;
		int config = *ptr;
		config |= (1 << 0);
		*ptr = config;
	} else if(cause==18){
		volatile int *  ptr;
		ptr=(int*)0x040000d0;
		ptr+=3;
		int config=*ptr;
		config|=1<<0;
		*ptr=config;
		newGame();
	
	}
	
	gameState();

	vga_draw_game();

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
  
  //switches
  volatile int *  ptr3;
  ptr3=(int*)0x04000010;
  ptr3+=2;
  *ptr3=771;

}
/* This is the main function */
int main()
{
	labinit();
	clear_screen();

	start();

	vga_draw_game(); 

  // Enter a forever loop
  while (1)
    { }
}
