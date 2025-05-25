#ifndef INPUT_H
#define INPUT_H

#include "common.h"

void setupTerminal(struct termios* new_tty,struct termios *original_termios); /* change terminal setting for non-canonical input */
void restoreTerminal(struct termios* original_termios); /* restore terminal to its original setting */
int readKey(void);           /* read arrow keys (and other single characters) */
void processKey(int* key_code, pos* cursor_pos); /* processing the movement of cursor based on key input (which direction it should go) */
void handleInput(int disc_counter[], int* key, enum Player turn, pos* cursor, pos* p); /* Handles key input for the player's move, updating the cursor position and validating the move on the game board */

#endif