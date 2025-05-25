#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "common.h"

pos* findOpposite(int r, int c); /* to find which positions around the square, have the opposite value */
int direction(int r, int c, pos opposite_pos); /* to find in which direction the opposite disc exist */
pos* find(int turn); /* return array of all possible moves for a given player at each turn*/
void possibleMove(int y, int x, pos** disc, size_t *len); /* to find possible moves for a single disc at (y,x) */
void printMoves(pos* p, int turn); /* print all possible moves for a player */
int checkValid(int y, int x, pos* p); /* check if the chosen move (y,x) is a valid move from the possible moves array p */
int confirm(int y, int x, int turn, pos* pm, int* counter, int board[][BOARD_SIZE]); /* turn the discs around after a valid move */
int goLeft(int y, int x, int turn);  /* check if on left side there are discs to flip */
int goRight(int y, int x, int turn); /* check if on right side there are discs to flip */
int goDown(int y, int x, int turn);  /* check if on downside there are discs to flip */
int goUp(int y, int x, int turn);    /* check if on upside there are discs to flip */
int goRightDown(int y, int x, int turn); /* check if on right_down side there are discs to flip */
int goLeftDown(int y, int x, int turn);  /* check if on left_down side there are discs to flip */
int goRightUp(int y, int x, int turn);   /* check if on right_up side there are discs to flip */
int goLeftUp(int y, int x, int turn);    /* check if on left_up side there are discs to flip */
int uniqueItem(int newItem_Y, int newItem_X, pos* arr, int len); /* check whether the new possible move already exists in the array */

#endif