#include "../include/game_flow.h"
#include "../include/board.h"
#include "../include/game_logic.h"
#include "../include/input.h"

void runGame(void) {
    pos *p;
    int disc_counter[2] = {2, 2}; // 0=>X, 1=>O
    char isBot;
    int key;

    gameSetup(&isBot, &key);

    enum Player turn = (key != 0 ? playLatestGame(disc_counter) : Player_O); // make the turn for previous match

    pos cursor = {.y = 7, .x=3};

    while (1) {
        clearScreen();
        if (win() == 1) {
            displayGameResult(disc_counter);
            break;
        }

        if (isBot == 'y') {
            p = find(turn);
            if (turn == Player_O) {
                handleInput(disc_counter, &key, turn, &cursor, p);
                free(p);
                key = 0;
            } else {
                clearScreen();
                drawBoard();
                compMove(p, disc_counter);
                usleep(1500 * 1000);

                free(p);
            }
        } else if (isBot == 'n') {
            p = find(turn);
            handleInput(disc_counter, &key, turn, &cursor, p);
            free(p);
            key = 0;
            usleep(100 * 1000);
        }
        turn = turn == Player_X ? Player_O : Player_X;
        history('w', turn);
    }
}

void gameSetup(char *isBot, int *key) {
    clearScreen();
    printf("bot? y(es) , n(o)\n");
    scanf(" %c", isBot);

    clearScreen();
    printf("wanna play previous game? (1 or 0): ");
    *key = getchar() - 48;
    fflush(stdin);
}

int win(void) {
    pos *p = find(1);
    if (p[0].x == -1) {
        free(p);
        p = find(2);
        if (p[0].x == -1) {
            free(p);
            return 1;
        }
    }
    free(p);
    return 0;
}

void displayGameResult(int disc_counter[]) {
    printf("number of X: %d\nnumber of O: %d\n\n", disc_counter[0], disc_counter[1]);
    if (disc_counter[0] == disc_counter[1]) {
        printf("ended in draw :|\n");
        return;
    }
    printf("winner: %c\n", (disc_counter[0] > disc_counter[1] ? 'X' : 'O'));
}

void compMove(pos *pm, int *disc_counter) {
    if (pm[0].x == -1) {
        printf("no move\n");
        return;
    }
    int copy_counter[2];
    int copyBoard[8][8];
    int bestChoice = 0;
    pos bestMove = pm[0];

    for (int i = 0; pm[i].x != -1; i++) {

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)              // reset the `copyBoard` with current discs
                copyBoard[i][j] = board[i][j];

        copy_counter[0] = disc_counter[0];                 // reset the
        copy_counter[1] = disc_counter[1];                 // copy counter

        confirm(pm[i].y, pm[i].x, Player_X, pm, copy_counter, copyBoard);
        if (bestChoice < copy_counter[0]) {
            bestChoice = copy_counter[0];
            bestMove = pm[i];
        }
    }
    confirm(bestMove.y, bestMove.x, Player_X, pm, disc_counter, board);
}

void options(void) {
    printf("=======================OPTIONS=======================\n");
    printf("1)Start Game\n");
    printf("2)show latest game\n");
    printf("3)Exit\n");
}

void history(char perm, int turn) {
    if (perm == 'w') {
        FILE *fp = fopen("history", "w");
        fprintf(fp, "turn: %d\n\n", turn);
        for (int row = 0; row < 8; row++) {
            if (!row) {
                fputs("  ", fp);
                for (int k = 1; k <= 8; k++)
                    fprintf(fp, "\033[90m%d \033[0m", k);
                fprintf(fp, "\n");
            }

            fprintf(fp, "\033[90m%d \033[0m", row + 1);
            for (int column = 0; column < 8; column++) {
                switch (board[row][column]) {
                    case 0:
                        fprintf(fp, "\033[33m- \033[0m");
                        break;
                    case 1:
                        fprintf(fp, "\033[31mX \033[0m");
                        break;
                    case 2:
                        fprintf(fp, "\033[36mO \033[0m");
                        break;
                }
            }
            fprintf(fp, "\n");
        }
        fclose(fp);

    } else if (perm == 'r') {
        FILE *fp = fopen("history", "r");
        if (fp == NULL) {
            printf("You haven't played a game.\n");
            return;
        }
        char line[10];
        while (fgets(line, 10, fp) != NULL) {
            printf("%s", line);
        }
        fclose(fp);
    }
}

int playLatestGame(int *counter) {

    FILE *fp = fopen("history", "r");
    if (fp == NULL)
        return 2;

    for (int k = 0; k < 6; k++)getc(fp);
    int turn = getc(fp) - 48;
    int i = 0, j = 0;
    char ch;
    counter[0] = 0;
    counter[1] = 0;

    while ((ch = getc(fp)) != EOF) {
        if (j >= 8) {
            i++;
            j = 0;
        }
        if (ch == '-') {
            board[i][j] = 0;
            j++;
        } else if (ch == 'X') {
            board[i][j] = 1;
            counter[0]++;
            j++;
        } else if (ch == 'O') {
            board[i][j] = 2;
            counter[1]++;
            j++;
        }
    }
    return turn;
}
