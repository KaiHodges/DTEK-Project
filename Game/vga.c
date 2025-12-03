#include <stdbool.h>
#include <stdint.h>
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

/* ---- 5x7 bitmap font helpers ---- */

static void draw_char(int x, int y, char c, unsigned char color)
{
    // Each glyph is 5 bits wide, 7 rows tall.
    // Bit 4 = leftmost pixel, bit 0 = rightmost.
    static const uint8_t GLYPH_SPACE[7] = {0,0,0,0,0,0,0};

    static const uint8_t GLYPH_T[7] = {
        0x1F, // 11111
        0x04, // 00100
        0x04,
        0x04,
        0x04,
        0x04,
        0x04
    };
    static const uint8_t GLYPH_E[7] = {
        0x1F, // 11111
        0x10, // 10000
        0x1E, // 11110
        0x10,
        0x1E,
        0x10,
        0x1F
    };
    static const uint8_t GLYPH_R[7] = {
        0x1E, // 11110
        0x11, // 10001
        0x1E,
        0x14, // 10100
        0x12, // 10010
        0x11,
        0x11
    };
    static const uint8_t GLYPH_I[7] = {
        0x0E, // 01110
        0x04, // 00100
        0x04,
        0x04,
        0x04,
        0x04,
        0x0E
    };
    static const uint8_t GLYPH_S[7] = {
        0x0F, // 01111
        0x10, // 10000
        0x0E, // 01110
        0x01, // 00001
        0x01,
        0x02,
        0x1E
    };
    static const uint8_t GLYPH_P[7] = {
        0x1E, // 11110
        0x11,
        0x1E,
        0x10,
        0x10,
        0x10,
        0x10
    };
    static const uint8_t GLYPH_B[7] = {
        0x1E, // 11110
        0x11,
        0x1E,
        0x11,
        0x11,
        0x11,
        0x1E
    };
    static const uint8_t GLYPH_U[7] = {
        0x11, // 10001
        0x11,
        0x11,
        0x11,
        0x11,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_O[7] = {
        0x0E, // 01110
        0x11,
        0x11,
        0x11,
        0x11,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_N[7] = {
        0x11, // 10001
        0x19, // 11001
        0x15, // 10101
        0x13, // 10011
        0x11,
        0x11,
        0x11
    };
    static const uint8_t GLYPH_A[7] = {
        0x0E, // 01110
        0x11,
        0x11,
        0x1F, // 11111
        0x11,
        0x11,
        0x11
    };
    static const uint8_t GLYPH_M[7] = {
        0x11, // 10001
        0x1B, // 11011
        0x15, // 10101
        0x11,
        0x11,
        0x11,
        0x11
    };
    static const uint8_t GLYPH_V[7] = {
        0x11, // 10001
        0x11,
        0x11,
        0x11,
        0x0A, // 01010
        0x0A,
        0x04  // 00100
    };
    static const uint8_t GLYPH_G[7] = {
        0x0E, // 01110
        0x11,
        0x10,
        0x17, // 10111
        0x11,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_C[7] = {
        0x0E, // 01110
        0x11,
        0x10,
        0x10,
        0x10,
        0x11,
        0x0E
    };

    // Digits 0-9
    static const uint8_t GLYPH_0[7] = {
        0x0E, // 01110
        0x11,
        0x13,
        0x15,
        0x19,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_1[7] = {
        0x04, // 00100
        0x0C, // 01100
        0x04,
        0x04,
        0x04,
        0x04,
        0x0E
    };
    static const uint8_t GLYPH_2[7] = {
        0x0E, // 01110
        0x11,
        0x01,
        0x06,
        0x08,
        0x10,
        0x1F
    };
    static const uint8_t GLYPH_3[7] = {
        0x1E, // 11110
        0x01,
        0x01,
        0x0E,
        0x01,
        0x01,
        0x1E
    };
    static const uint8_t GLYPH_4[7] = {
        0x02, // 00010
        0x06,
        0x0A,
        0x12,
        0x1F,
        0x02,
        0x02
    };
    static const uint8_t GLYPH_5[7] = {
        0x1F, // 11111
        0x10,
        0x1E,
        0x01,
        0x01,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_6[7] = {
        0x06, // 00110
        0x08,
        0x10,
        0x1E,
        0x11,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_7[7] = {
        0x1F, // 11111
        0x01,
        0x02,
        0x04,
        0x08,
        0x08,
        0x08
    };
    static const uint8_t GLYPH_8[7] = {
        0x0E, // 01110
        0x11,
        0x11,
        0x0E,
        0x11,
        0x11,
        0x0E
    };
    static const uint8_t GLYPH_9[7] = {
        0x0E, // 01110
        0x11,
        0x11,
        0x0F,
        0x01,
        0x02,
        0x0C
    };

    static const uint8_t GLYPH_COLON[7] = {
        0x00,
        0x04,
        0x04,
        0x00,
        0x04,
        0x04,
        0x00
    };

    const uint8_t *glyph = GLYPH_SPACE;

    // Force uppercase for letters
    if (c >= 'a' && c <= 'z') {
        c = (char)(c - 'a' + 'A');
    }

    switch (c) {
        case 'T': glyph = GLYPH_T; break;
        case 'E': glyph = GLYPH_E; break;
        case 'R': glyph = GLYPH_R; break;
        case 'I': glyph = GLYPH_I; break;
        case 'S': glyph = GLYPH_S; break;
        case 'P': glyph = GLYPH_P; break;
        case 'B': glyph = GLYPH_B; break;
        case 'U': glyph = GLYPH_U; break;
        case 'O': glyph = GLYPH_O; break;
        case 'N': glyph = GLYPH_N; break;
        case 'A': glyph = GLYPH_A; break;
        case 'M': glyph = GLYPH_M; break;
        case 'V': glyph = GLYPH_V; break;
        case 'G': glyph = GLYPH_G; break;
        case 'C': glyph = GLYPH_C; break;

        case '0': glyph = GLYPH_0; break;
        case '1': glyph = GLYPH_1; break;
        case '2': glyph = GLYPH_2; break;
        case '3': glyph = GLYPH_3; break;
        case '4': glyph = GLYPH_4; break;
        case '5': glyph = GLYPH_5; break;
        case '6': glyph = GLYPH_6; break;
        case '7': glyph = GLYPH_7; break;
        case '8': glyph = GLYPH_8; break;
        case '9': glyph = GLYPH_9; break;

        case ':': glyph = GLYPH_COLON; break;

        case ' ':
        default:  glyph = GLYPH_SPACE; break;
    }

    // draw 5x7 pixels
    for (int row = 0; row < 7; row++) {
        for (int col = 0; col < 5; col++) {
            if (glyph[row] & (1 << (4 - col))) {   // bit 4 is leftmost
                put_pixel(x + col, y + row, color);
            }
        }
    }
}


static void draw_string(int x, int y, const char *s, unsigned char color)
{
    const int CHAR_W = 6;  // 5px glyph + 1px space
    while (*s) {
        draw_char(x, y, *s, color);
        x += CHAR_W;
        s++;
    }
}

static void int_to_str(int value, char *buf)
{
    // value assumed >= 0 for score
    if (value == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char tmp[12];  // enough for 32-bit int
    int pos = 0;

    while (value > 0 && pos < (int)sizeof(tmp)-1) {
        int digit = value % 10;
        tmp[pos++] = (char)('0' + digit);
        value /= 10;
    }

    // reverse into buf
    int i = 0;
    while (pos > 0) {
        buf[i++] = tmp[--pos];
    }
    buf[i] = '\0';
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

static int center_x_for_text(const char *s)
{
    int len = 0;
    while (s[len] != '\0') len++;

    int pixel_width = len * 6;          // 5px glyph + 1px spacing
    return (SCREEN_WIDTH - pixel_width) / 2;
}

void draw_startscreen(void) 
{
    clear_screen();

    // TETRIS
    const char *title = "TETRIS";
    int title_x = center_x_for_text(title);
    int title_y = 70;
    draw_string(title_x, title_y, title, COLOR_CYAN_FILL);

    // PRESS BUTTON 1 TO START
    const char *msg = "PRESS BUTTON 1 TO START";
    int msg_x = center_x_for_text(msg);
    int msg_y = 130;
    draw_string(msg_x, msg_y, msg, COLOR_YELLOW_FILL);
}


void draw_gameover_screen(int final_score)
{
    clear_screen();

  // GAME OVER 
	const char *title = "GAME OVER";
    int title_x = center_x_for_text(title);
    int title_y = 60;
    draw_string(title_x, title_y, title, COLOR_RED_FILL);
		
		// SCORE
    const char *label = "SCORE:";
    int label_x = center_x_for_text(label);
    int label_y = 110;
    draw_string(label_x, label_y, label, COLOR_YELLOW_FILL);

    // Score Value 
    char score_buf[16];
    int_to_str(final_score, score_buf);

    int score_x = center_x_for_text(score_buf);
    int score_y = label_y + 20;   // separation below SCORE:
    draw_string(score_x, score_y, score_buf, COLOR_YELLOW_FILL);

    // BUTTON 1 TO RESTART 
    const char *restart = "BUTTON 1 TO RESTART";
    int restart_x = center_x_for_text(restart);
    int restart_y = 170;
    draw_string(restart_x, restart_y, restart, COLOR_CYAN_FILL);
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


void vga_draw_startscreen(void) 
{
	clear_screen();
	draw_startscreen();
	vga_show_frame();
}

void vga_draw_gameover(int final_score)
{
	draw_gameover_screen(final_score);
  vga_show_frame();
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

    // show the frame
    vga_show_frame();
}




