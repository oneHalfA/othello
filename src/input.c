#include "../include/input.h"
#include "../include/board.h"
#include "../include/game_logic.h"

void setupTerminal(struct termios* tty, struct termios *original_tty) {
    tcgetattr(STDIN_FILENO, tty);
    *original_tty = *tty;
    tty->c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, tty);
    printf("\033[?25l");
    fflush(stdout);
}
void restoreTerminal(struct termios* org_tty) {
    tcsetattr(STDIN_FILENO, TCSANOW, org_tty);
    printf("\033[?25h");
    fflush(stdout);
}
int readKey(void){
    int ch = getchar();
    if(ch == 27){
        printf("z");
        getchar();
        ch = getchar();
    }
    return ch;
}
void processKey(int* key,pos* cursor){
    switch(*key){
        case KEY_RIGHT:
            cursor->x = cursor->x + 2 <= BORDER_R? cursor->x + 2 : cursor->x;
            break;
        case KEY_LEFT:
            cursor->x = cursor->x - 2 >= BORDER_L ? cursor->x - 2 : cursor->x;
            break;
        case KEY_UP:
            cursor->y = cursor->y - 1 >= BORDER_U ? cursor->y - 1 : cursor->y;
            break;
        case KEY_DOWN:
            cursor->y = cursor->y + 1 <= BORDER_D ? cursor->y + 1 : cursor->y;
            break;
    }
}
void handleInput(int disc_counter[],int* key, enum Player turn, pos* cursor, pos* p){

    if(p[0].x == -1){ // check if there is possible move
        return;
    }
    while(*key != '\n'){
        clearScreen();
        printf("\n\nnumber of X: %d\nnumber of O: %d\n\n",
               disc_counter[0],disc_counter[1]);
        drawBoard();
        printMoves(p,turn);
        drawCursor(cursor,turn);
        fflush(stdout);
        *key = readKey();
        processKey(key, cursor);
    }
    if(!confirm(cursor->y-7,cursor->x/2-1,turn,p,disc_counter,board)){
        printf("\033[20;10HNot valid Disc!!\n");
        usleep(1000*1000);
        *key = 0;
        handleInput(disc_counter,key,turn,cursor,p);
    }
    board[cursor->y - 7][cursor->x / 2 - 1] = turn;
}
