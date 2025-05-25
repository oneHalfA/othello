#include "../include/game_flow.h" // Includes runGame()
#include "../include/board.h"
#include "../include/input.h"

int board[BOARD_SIZE][BOARD_SIZE] = {0};

int main(void) {
    struct termios original_tty;
    struct termios tty;
    setupTerminal(&tty,&original_tty);

    int key;
    while (1) {

        initializeBoard();
        options();
        key = (int) getchar() - 48;

        switch (key) {
            case 1: //game
                clearScreen();
                printf("===============================WELCOME===============================\n\n");
                runGame();
                break;
            case 2: //latest game
                clearScreen();
                history('r', 0); // 0 here do nothing
                break;
        }
        if (key == 3)
            break;
    }
    restoreTerminal(&original_tty);
    return 0;
}
