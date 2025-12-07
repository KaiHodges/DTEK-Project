//written by Kai Hodges and Lucas Dahlgren with Lab3 as a base
#include "gameState.h"
#include "shapes.h" // for struct shapes
#include "vga.h"

extern void enable_interrupt(void); // needs to be declared for the compiler

void newGame(void) { start(); }
void gameOver(void) { gameon = 0; }

void handle_interrupt(unsigned cause) {
  seed++;
  if (cause == 16 && gameon == 1) {
    volatile short *volatile TO;
    TO = (short *)0x04000020;
    *TO = 0;
    invocations++;
    if (invocations == 3 / mult) {
      invocations = 0;
      if (reachedBottom() == 1) {
        lockPos();
        score(checkRows());
        if (gameOverCheck() == 0) {
          gameOver();
          vga_draw_gameover(scores);
        } else {
          newBlock();
        }
      } else {
        down();
      }
    }
  } else if (cause == 17 && gameon == 1) {
    volatile int *ptr = (int *)0x04000010;
    ptr += 3;
    int switchState = *ptr;
    *ptr = 0xFFFFFFFF;

    if (switchState & 0x1) {
      right();
    }
    if (switchState & 0x2) {
      rotate();
    }
    if (switchState & 0x100) {
      store();
    }
    if (switchState & 0x200) {
      left();
    }
    for(int i =0; i<4;i++){}

  } else if (cause == 18) {
    volatile int *ptr;
    ptr = (int *)0x040000d0;
    ptr += 3;
    int config = *ptr;
    config |= 1 << 0;
    *ptr = config;

		if (gameon == 2 || gameon == 0) {
			newGame();
			}
      if(gameon==1){
        if (reachedBottom() == 1) {
        lockPos();
        score(checkRows());
        if (gameOverCheck() == 0) {
          gameOver();
          vga_draw_gameover(scores);
        } else {
          newBlock();
        }
      } else {
        down();
      }
    }
	}

	if (gameon == 2) {
		vga_draw_startscreen();
	} else if	(gameon == 1) {
			vga_draw_game();
		} else if (gameon == 0) {
			vga_draw_gameover(scores);
			}
			
}

/**
 *
 *
 */
void labinit(void) {
  enable_interrupt();
  volatile int *volatile ptr;
  ptr = (int *)0x4000020;
  *ptr = 0;
  int time = 3000000; // 30 Mhz/10 is 3 million actions to get 100 ms
  int periodh = time >> 16;
  int periodl = time & 0xffff;
  ptr += 2;
  *ptr = periodl;
  ptr += 1;
  *ptr = periodh;
  ptr -= 2;
  int config = *ptr;
  config |= 1 << 2; // bitwise logic to set bitt <<k to one
  config |= 1 << 1;
  config |= 1 << 0;
  *ptr = config;

  // buttons
  volatile int *ptr2;
  ptr2 = (int *)0x040000d0;
  ptr2 += 2;
  *ptr2 = 1;

  // switches
  volatile int *ptr3;
  ptr3 = (int *)0x04000010;
  ptr3 += 2;
  *ptr3 = 771;
}
/* This is the main function */
int main() {
  labinit();
  clear_screen();

  vga_draw_startscreen();
  // Enter a forever game loop
  while (1) {
  }
}
