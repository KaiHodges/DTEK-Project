#include <stdio.h>
#include <stdlib.h>
#include "vga.h"

#define CELL_SIZE   12
#define GRID_COLS   10
#define GRID_ROWS   20
#define GRID_LEFT   100
#define GRID_TOP    0

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

void clear_screen()
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        frame0[i] = COLOR_BLACK; 
    }
}

static unsigned char darker(unsigned char c)
{
    // Extract RGB components
    unsigned char r = (c >> 5) & 0x7;
    unsigned char g = (c >> 2) & 0x7;
    unsigned char b =  c       & 0x3;

    // Darken them safely (prevent underflow)
    if (r > 0) r-= 2;
    if (g > 0) g-= 2;
    if (b > 0) b--;

    // Pack back into RGB332
    return (r << 5) | (g << 2) | b;
}

// fill a square in the grid with the chosen color
void fill_square(int x, int y, unsigned char color) 
{		
		int x_start = 12 * x + GRID_LEFT + 1; 
		int y_start = 12 * y; 
		
		unsigned char border = darker(color); 

		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {

				bool is_border = ((i == 0) || (i == 10) || (j == 0) || (j == 10));
				bool is_corner = ((i == 1 && j == 1) || (i == 1 && j == 9) || (i == 9 && j == 1) || (i == 9 && j == 9)); 
				
      	if ((i == 0 || i == 10) && (j == 0 || j == 10)) {continue; } // Skip the four corner pixels
				
				if (is_border || is_corner) 
					put_pixel(x_start + i, y_start + j, border);
				else 
					put_pixel(x_start + i, y_start + j, color);
			} 
		} 
}

void clear_square(int x, int y) 
{
	fill_square(x,y,COLOR_BLACK);
}

void draw_grid(void) 
{	
    int grid_width  = GRID_COLS * CELL_SIZE;
    int grid_height = GRID_ROWS * CELL_SIZE;

    int x_start = GRID_LEFT;
    int y_start = GRID_TOP;
    int x_end   = x_start + grid_width - 1;
    int y_end   = y_start + grid_height - 1;

    // Draw vertical lines
    for (int col = 0; col <= GRID_COLS; col++) {
        int x = x_start + col * CELL_SIZE;
        if (x >= SCREEN_WIDTH) continue;

        for (int y = y_start; y <= y_end && y < SCREEN_HEIGHT; y++) {
            put_pixel(x, y, COLOR_GRID);
        }
    }

    // Draw horizontal lines
    for (int row = 0; row <= GRID_ROWS; row++) {
        int y = (y_start + row * CELL_SIZE) - 1;
        if (y >= SCREEN_HEIGHT) {y = SCREEN_HEIGHT - 1;} //changed so the bottom line is included 

        for (int x = x_start; x <= x_end && x < SCREEN_WIDTH; x++) {
            put_pixel(x, y, COLOR_GRID);
        }
    }
}

void draw_background(void) 
{
	draw_grid(); 

	for (int x = 0; x < GRID_LEFT; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			put_pixel(x,y,COLOR_BACKGROUND); }
		}
	
	for (int x = GRID_LEFT + 120 + 1; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			put_pixel(x,y,COLOR_BACKGROUND); }
		}
		
}




void vga_show_frame(void)
{
    vga_ctrl[1] = (unsigned int)frame0; // BackBuffer = frame0
    vga_ctrl[0] = 0;                    // trigger swap
}

