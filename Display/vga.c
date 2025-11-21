#define VGA_PIXEL_BASE 0x08000000; //framebuffer base, memory address for the framebuffer 
#define VGA_DMA_BASE 0x04000100; //address for DMA ctrl interactions


static volatile unsigned char* const VGA_PIXEL = (unsigned char*) VGA_PIXEL_BASE;
static volatile unsigned int* const VGA_CTRL = (unsigned char*) VGA_DMA_BASE;


void place_pixel(int x, int y, unsigned char color) 
{

	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) 
		return; 
		
	VGA_PIXEL[y * SCREEN_WIDTH + x] = color; 

} 

void clear_screen(unsigned char color)
{
 for(int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			VGA_PIXEL[y * SCREEN_WIDTH + x] = color;
		}

	}

}
