#include "../include/board.h"   // Include its own header first

void drawBoard(void){
    for(int row=0;row<8;row++){
        if(!row){
            printf("  ");
            for(int k=1;k<=8;k++)
                printf("\033[90m%d \033[0m",k);
            printf("\n");
        }

        printf("\033[90m%d \033[0m",row+1);
        for(int column=0;column<8;column++){
            switch(board[row][column]){
                case 0:
                    printf("\033[33m- \033[0m");
                    break;
                case 1:
                    printf("\033[31mX \033[0m");
                    break;
                case 2:
                    printf("\033[36mO \033[0m");
                    break;
            }
        }
        printf("\n");
    }
}
void drawCursor(pos* cursor,int turn){
    printf("%d %d\n",cursor->x/2,cursor->y-6);
    printf("\033[%d;%dH%c",cursor->y,cursor->x,
           turn == Player_X ? 'X':'O'
    );
}
void initializeBoard(void){
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            board[i][j] = 0;
    board[3][3] = 1;	board[3][4] = 2;	board[4][3] = 2;	board[4][4] = 1;
}
void clearScreen(void){
    printf("\033[2J\033[H");
}