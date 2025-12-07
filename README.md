# DTEK-Project - Tetris
Project created as part of the IS1500 course "Computer Organization and Components" 

## Running the program
Asuming you have the neccesary software to use the DTEK-V board all you need to do is move to the Game folder and run "make", before running the program using "../dtekv-tools/dtekv-run main.bin" while in the Game folder

## Controls
Button 1: starts the game when on the start screen or on the gameOver screen, while in the game it works as a down button.
Swithces
Rightmost switch (01): moves the block to the right if it is legal
Second rightmost switch (02): rotates the block if it is legal
Leftmost switch (09): moves the block to the left if it is legal
Seconf leftmost switch(08): stores the block, can only be done once per new block.  

## Contributions
The lab 3 code from the IS1500 course at KTH laid the ground works for the project.
Files rewritten include boot.s where interupts where changed to enable the switches interupt. Main.c where functionality was both expanded for interupts, and added such as game over and start.

The shapes.h gameState.h and gameState.c was written by Lucas Dahlgren with some aid from Kai Hodges. The exception is one LCG algotihm in gameState.c specifics can be found in the file.

The vga.h and vga.c was written by Kai Hodges.