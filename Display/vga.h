#ifndef VGA_H
#define VGA_H

typedef struct {
  unsigned char fill;
  unsigned char border;
} BlockColor;

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Grid / cell layout (for Tetris board)
#define CELL_SIZE 12
#define GRID_COLS 10
#define GRID_ROWS 20
#define GRID_LEFT 100
#define GRID_TOP 0

// Convert 3:3:2 RGB to one byte (RGB332)
#define RGB332(r, g, b) (((r & 0x7) << 5) | ((g & 0x7) << 2) | ((b & 0x3)))

// Basic colors
#define COLOR_BLACK RGB332(0, 0, 0)
#define COLOR_WHITE RGB332(7, 7, 3)
#define COLOR_GRID RGB332(5, 5, 2)
#define COLOR_BACKGROUND RGB332(3, 4, 2)

#define COLOR_RED_FILL RGB332(5, 0, 0)
#define COLOR_RED_BORDER RGB332(3, 0, 0)
extern const BlockColor COLOR_RED;

#define COLOR_BLUE_FILL RGB332(0, 0, 2)
#define COLOR_BLUE_BORDER RGB332(0, 0, 1)
extern const BlockColor COLOR_BLUE;

#define COLOR_GREEN_FILL RGB332(0, 5, 0)
#define COLOR_GREEN_BORDER RGB332(0, 3, 0)
extern const BlockColor COLOR_GREEN;

// Framebuffers and control registers (defined in vga.c)
extern volatile unsigned char *frame0;
extern volatile unsigned char *frame1;
extern volatile unsigned int *vga_ctrl;

// Drawing functions
void put_pixel(int x, int y, unsigned char color);
void clear_screen();
void fill_square(int x, int y, BlockColor color);
void draw_grid(void);
void draw_background(void);

// Swap to show current frame
void vga_show_frame(void);

#endif // VGA_H
