#include <stdio.h>
#include <stdlib.h>
#include "vga.h"

#define CELL_SIZE   12
#define GRID_COLS   10
#define GRID_ROWS   20
#define GRID_LEFT   100
#define GRID_TOP    20

volatile unsigned char *frame0 = (unsigned char*) 0x08000000;
volatile unsigned char *frame1 = (unsigned char*) 0x08000000 + SCREEN_WIDTH * SCREEN_HEIGHT;
volatile unsigned int  *vga_ctrl = (unsigned int*) 0x04000100;

void put_pixel(int x, int y, unsigned char color) 
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    } 
		
    frame0[y * SCREEN_WIDTH + x] = color; 
} 

void clear_screen(unsigned char color)
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        frame0[i] = color; 
    }
}

// Draw a 12x12 pixel square outline
void draw_square(int x, int y, unsigned char color) 
{	
    int x_end = x + CELL_SIZE - 1; 
    int y_end = y + CELL_SIZE - 1; 

    for (int xx = x; xx <= x_end; xx++) {
        put_pixel(xx, y,     color);
        put_pixel(xx, y_end, color); 
    }
		
    for (int yy = y; yy <= y_end; yy++) {
        put_pixel(x,     yy, color);
        put_pixel(x_end, yy, color);
    }
}

void draw_grid(void) 
{	
    for (int col = 0; col < GRID_COLS; col++) {
        for (int row = 0; row < GRID_ROWS; row++) {
            int x = GRID_LEFT + col * CELL_SIZE;
            int y = GRID_TOP  + row * CELL_SIZE;
            draw_square(x, y, COLOR_WHITE);
        }
    }
}

