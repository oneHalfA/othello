#ifndef BOARD_H
#define BOARD_H

#include "common.h"

void drawBoard(void);       /* to draw the board */
void initializeBoard(void); /* Initialize the board for new game */
void drawCursor(pos* cursor_pos, int currentPlayer); /* Draw cursor */
void clearScreen(void);     /* Clear screen with ansi codes */

#endif