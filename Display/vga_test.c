#include <stdint.h>

//int main(void)
//{
//    // Pixel buffer base
//    volatile unsigned char *VGA = (volatile unsigned char*) 0x08000000;
//    // VGA Pixel Buffer DMA control
//    volatile int *VGA_CTRL = (volatile int*) 0x04000100;
//
//    // Fill the entire pixel buffer region with a simple pattern
//    // 320 * 480 bytes (2x the visible height) = 153600
//    for (int i = 0; i < 320 * 480; i++) {
//        VGA[i] = i / 320;   // vertical gradient
//    }
//
//    unsigned int y_ofs = 0;
//
//    while (1) {
//        // BackBuffer = VGA + y_ofs*320
//        VGA_CTRL[1] = (unsigned int)(VGA + y_ofs * 320);
//        // Write to Buffer register to request swap
//        VGA_CTRL[0] = 0;
//
//        // Scroll down by 1 line (wrap 0..239)
//        y_ofs = (y_ofs + 1) % 240;
//
//        // crude delay
//        for (volatile int i = 0; i < 1000000; i++) {
//            __asm__ volatile("nop");
//        }
//    }
//
//    return 0;
//}

