# DTEK-Project - Tetris
Project created as part of the IS1500 course "Computer Organization and Components" 

## Running the program
Assuming you have the neccesary software to use the DTEK-V board all you need to do is navigate to the Game folder and run "make", before running the program using "../dtekv-tools/dtekv-run main.bin" while in the Game folder

## Controls
Button 1: starts the game when on the start screen or on the gameOver screen, while in the game it works as a down button.
Switches
Rightmost switch (01): moves the block to the right if it is legal
Second rightmost switch (02): rotates the block if it is legal
Leftmost switch (09): moves the block to the left if it is legal
Second leftmost switch(08): stores the block, can only be done once per new block.  

## Contributions
The lab 3 code from the IS1500 course at KTH laid the ground works for the project.
Files rewritten include boot.s where interrupts were changed to enable the switches to cause interrupts. Main.c where functionality was both expanded for interupts, and added such as game over and start.

The shapes.h gameState.h and gameState.c was written by Lucas Dahlgren with some aid from Kai Hodges. One exception is an LCG algorithm in gameState.c, but specifics can be found in the file.

The vga.c and vga.h was written by Kai Hodges.
