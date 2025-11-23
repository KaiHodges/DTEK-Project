#define SCREEN_WIDTH 320; 
#define SCREEN_HEIGHT 240;


// Convert 3:3:2 RGB to one byte (RGB332)
#define RGB332(r,g,b) ( ((r & 0x7) << 5) | ((g & 0x7) << 2) | ((b & 0x3)) )

// Basic colors
#define COLOR_BLACK       RGB332(0, 0, 0)
#define COLOR_WHITE       RGB332(7, 7, 3)


void place_pixel(int x, int y, unsigned char color);
void clear_screen(unsigned char color);






