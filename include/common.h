#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>


#define R 1
#define L 2
#define U 3
#define D 4
#define RU 13
#define RD 14
#define LU 23
#define LD 24
#define BORDER_R 17
#define BORDER_L 3
#define BORDER_U 7
#define BORDER_D 14
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define KEY_UP 65
#define KEY_DOWN 66
#define BOARD_SIZE 8

typedef struct pos{
    int x,y;
} pos;

extern int board[BOARD_SIZE][BOARD_SIZE];

enum Player {
    Player_X = 1,
    Player_O = 2
};



#endif