#ifndef GAME_FLOW_H
#define GAME_FLOW_H

#include "common.h"

void runGame(void);          /* Main logic of the game*/
void gameSetup(char *isBot, int *play_prev_game); /*Different options of playing (like PVP and PVE)*/
int win(void);               /* Check if the board is filled and no empty disc exist, returns 1 if game is finished */
void displayGameResult(int *disc_counter); /* Show the result if the game is finished */
void compMove(pos* possible_moves, int *disc_counter); /* Computer move if playing against bot */
void options(void);          /* Displays main menu options */
void history(char perm, int turn); /* Write or read the last played game board (mode 'w'/'r') */
int playLatestGame(int *disc_counter); /* To load and continue playing the latest game */

#endif