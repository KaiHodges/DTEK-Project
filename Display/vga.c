#include <stdbool.h>
#include "vga.h"
#include "gameState.h"

#define CELL_SIZE   12
#define GRID_COLS   10
#define GRID_ROWS   20
#define GRID_LEFT   100
#define GRID_TOP    0

//Structs for block colors
const BlockColor COLOR_RED   = { COLOR_RED_FILL,   COLOR_RED_BORDER };
const BlockColor COLOR_BLUE  = { COLOR_BLUE_FILL,  COLOR_BLUE_BORDER };
const BlockColor COLOR_GREEN = { COLOR_GREEN_FILL, COLOR_GREEN_BORDER };
const BlockColor COLOR_YELLOW  = { COLOR_YELLOW_FILL,  COLOR_YELLOW_BORDER };
const BlockColor COLOR_CYAN    = { COLOR_CYAN_FILL,    COLOR_CYAN_BORDER };
const BlockColor COLOR_PURPLE  = { COLOR_PURPLE_FILL,  COLOR_PURPLE_BORDER };
const BlockColor COLOR_ORANGE  = { COLOR_ORANGE_FILL,  COLOR_ORANGE_BORDER };

static BlockColor color_for_type(int type)
{
    switch (type) {
        case 1: return COLOR_CYAN;    // I
        case 2: return COLOR_YELLOW;  // O
        case 3: return COLOR_ORANGE;  // L
        case 4: return COLOR_BLUE;    // J
        case 5: return COLOR_PURPLE;  // T
        case 6: return COLOR_GREEN;   // S
        case 7: return COLOR_RED;     // Z
        default: {
            BlockColor black = { COLOR_BLACK, COLOR_BLACK };
            return black;
        }
    }
}


/* pointers to image buffers to create the images,
 * also a pointer to vga controls to choose image being displayed. 
*/
volatile unsigned char *frame0 = (unsigned char*) 0x08000000;
volatile unsigned char *frame1 = (unsigned char*) 0x08000000 + SCREEN_WIDTH * SCREEN_HEIGHT;
volatile unsigned int  *vga_ctrl = (unsigned int*) 0x04000100;

/* Pointer to the buffer we are currently drawing into (back buffer). */
volatile unsigned char *draw_frame = (unsigned char*)0x08000000;  // start with frame0

/* function to place a pixel at the specified (x,y) coordinate,
 * (0,0) is the top left of the screen, increasing in x and y values going right and down respectively. 
*/ 
void put_pixel(int x, int y, unsigned char color) 
{
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    } 
		
		draw_frame[y * SCREEN_WIDTH + x] = color; 
} 

/* Helper function to clear the screen. 
*/
void clear_screen()
{
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        draw_frame[i] = COLOR_BLACK; 
    }
}

// fill a square in the grid with the chosen color
void fill_square(int x, int y, BlockColor color) 
{		
		int x_start = 12 * x + GRID_LEFT + 1; 
		int y_start = 12 * y; 
		
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 11; j++) {

				bool is_border = ((i == 0) || (i == 10) || (j == 0) || (j == 10));
				bool is_corner = ((i == 1 && j == 1) || (i == 1 && j == 9) || (i == 9 && j == 1) || (i == 9 && j == 9)); 
				
      	if ((i == 0 || i == 10) && (j == 0 || j == 10)) {continue; } // Skip the four corner pixels
				
				if (is_border || is_corner) 
					put_pixel(x_start + i, y_start + j, color.border);
				else 
					put_pixel(x_start + i, y_start + j, color.fill);
			} 
		} 
}

void clear_square(int x, int y) 
{
	BlockColor BLACK = {COLOR_BLACK, COLOR_BLACK};
	fill_square(x,y,BLACK);
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
	// tell VGA which buffer we want to show next (the one we just drew into)
    vga_ctrl[1] = (unsigned int)draw_frame;
    vga_ctrl[0] = 0;  // trigger hardware swap

    // now swap our software notion of back buffer
    if (draw_frame == frame0) {
        draw_frame = frame1;
    } else {
        draw_frame = frame0;
    }
}

void vga_draw_game(void)
{
    clear_screen();    // clear back buffer and draw static background
    draw_background();

    // draw all locked blocks from the playing grid
    for (int x = 0; x < GRID_COLS; x++) {
        for (int y = 0; y < GRID_ROWS; y++) {
            int type = playingGrid[x][y]; // 0 = empty, >0 = block type
            if (type != 0) {
                BlockColor c = color_for_type(type);
                fill_square(x, y, c);
            }
        }
    }

    // draw current falling shape 
    if (currentshape.type > 0) {
        BlockColor cur = color_for_type(currentshape.type);
        for (int i = 0; i < 4; i++) {
            int x = currentshape.x[i];
            int y = currentshape.y[i];
            if (y >= 0 && y < GRID_ROWS) {   // ignore pieces above top
                fill_square(x, y, cur);
            }
        }
    }

    // gonna put to draw stored piece somewhere off to the side here too 

    // show the frame
    vga_show_frame();
}




