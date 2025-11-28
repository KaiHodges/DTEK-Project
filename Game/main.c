#include "vga.h"

void handle_interrupt(unsigned _irq) {}

/* This is the main function */
int main()
{
	clear_screen();

	draw_background();
	
	fill_square(3,10,COLOR_RED);
	fill_square(4,10,COLOR_RED); 
	fill_square(5,10,COLOR_RED);
	fill_square(4,9,COLOR_RED);


	vga_show_frame();

  // Enter a forever loop
  while (1)
    { }
}
