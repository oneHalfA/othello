#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
// ======================================MARCOS/VARABLE/STRUCTURES

typedef struct pos{
	int x,y;
} pos;

int board[8][8] = {{0}};

enum Player { 
    Player_X = 1,
    Player_O = 2
};

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
// ======================================FUNCTION_PROTOTYPES

void drawBoard(void); /* to draw the board */

pos* findOpposite(int, int); /* to find which positions around the square, have the opposite value */

int direction(int ,int ,pos); /* to find in which direction the opposite disc exist */

pos* find(int ); /* return array of all possible move at each turn*/

void possibleMove(int ,int ,pos** ,size_t *); /* to find possible move for single disc*/

void printMoves(pos*, int); /*print all possible moves*/

int checkValid(int ,int ,pos*); /* check if the chosen move is a valid move */

int admit(int ,int ,int ,pos*,int*,int [][8]); /*turn the discs around*/

int goLeft(int ,int ,int); /* check if on left side there are discs need to flip them over*/

int goRight(int ,int ,int);/* check if on right side there are discs need to flip them over*/

int goDown(int ,int ,int);/* check if on down side there are discs need to flip them over*/

int goUp(int ,int ,int);/* check if on up side there are discs need to be flflip them over*/

int goRightDown(int ,int ,int);/* check if on right_down side there are discs need to flip them over*/

int goLeftDown(int ,int ,int);/* check if on left_down side there are discs need to flip them over*/

int goRightUp(int ,int ,int);/* check if on right_up side there are discs need to flip them over*/

int goLeftUp(int ,int ,int);/* check if on left_up side there are discs need to flip them over*/

int uniqueItem(int, int, pos*,int); /* check wether the new possible move already exist in array*/

void compMove(pos* ,int *); /*computer move if playing bot*/

void options(void); /*different modes of this program*/

void history(char,int); /*write or read latest game board*/

void setupTerminal(struct termios* ); /*change terminal setting*/

void restoreTerminal(struct termios* ); /*restore terminal to its original setting*/

void clearScreen(void); /*clear screen with ansi codes*/

int playLatestGame(int *); /*to play the latest game*/

int win(void); /*check if the board is filled and no empty disc exist return 1*/

int readKey(void);  /*read arrow keys*/

void drawCursor(pos*,int); /*drawing cursor*/

void processKey(int*,pos*); /*processing the movement of cursor (which way it should go)*/

void handleInput(int *,int* , enum Player , pos* , pos* ); /*Handles key input for the player's move, updating the cursor position and validating the move on the game board*/

void gameSetup(char *,int *); /*different options of playing (like PVP and PVE)*/

void displayGameResult(int *); /*Show the result if the game is finished*/

void runGame(void); /*main logic of the game*/

void initializeBoard(void);/*initialize the board for new game*/
// ======================================MAIN_FUNCTION
int main(void){

    struct termios tty;
    setupTerminal(&tty); 

    int key;
    while(1){

	initializeBoard();
	options();
	key = (int)getchar() - 48;
	
	switch(key){
	    case 1: //game
		clearScreen();
		printf("===============================WELCOME===============================\n\n");
		runGame();
		break;
	    case 2: //latest game
		clearScreen();
		history('r',0); // 0 here do nothing 
		break;
	}	
	if(key == 3)
	    break;
    }
    restoreTerminal(&tty);
    return 0;
}
// ======================================FUNCTION_DEFINTITION
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

pos* findOpposite(int y,int x){
    size_t len = 1;
    int turn = board[y][x];
    pos* p = (pos*) malloc(1*sizeof(pos));
    p[0].x = -1;
    for(int j=y-1;j<y+2 ;j++){
	if(j<0 || j>=8)
	    continue;
	for(int i=x-1;i<x+2;i++){
	    if(i<0 || i>=8)
		continue;
	    if (board[j][i] == (turn == 1 ? 2:1)){
		p[len - 1].x = i;
		p[len - 1].y = j;
		len++;
		p =  realloc(p,len*sizeof(pos));
		p[len-1].x = -1;
	    }
	}
    }
    return p;
}

int direction(int y,int x, pos p){
    if(y == p.y){
	if(x == p.x - 1)
	    return R; // 1
	else 
	    return L; // 2
    }else if(x == p.x){
	if(y == p.y - 1)
	    return D; // 4
	else 
	    return U; // 3
    }else if(y == p.y - 1){
	if(x == p.x - 1)
	    return RD; // 14
	else
	    return LD; // 24
    }else if(y == p.y + 1){
	if(x == p.x + 1)
	    return LU; // 23
	else 
	    return RU; // 13
    }
    return -1;
}

pos* find(int turn){
    size_t len = 1;
    pos* tmp = (pos*)malloc(sizeof(pos)*len);
    tmp[len-1].x = -1;
    for(int i=0;i<8;i++){
	for(int j=0;j<8;j++){
	    if(board[i][j] == turn)
		possibleMove(i,j,&tmp,&len);
	}
    }
    printf("\n");
    return tmp;
}


void possibleMove(int y,int x,pos** disc,size_t *len){
    int turn = board[y][x];
    pos* p = findOpposite(y,x);
    for(int item=0;p[item].x!=-1;item++){
	switch(direction(y,x,p[item])){
	    case L:
		for(int i=p[item].x;i>=0;i--){
		    if(board[y][i] == turn)
			break;
		    if(board[y][i] == 0){
			if(uniqueItem(y,i,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = i;
			(*disc)[*len - 1].y = y;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len-1].x = -1;
			break;
		    }
		}
		break;
	    case R:
		for(int i=p[item].x;i<8;i++){
		    if(board[y][i] == turn)
			break;
		    if(board[y][i] == 0){
			if(uniqueItem(y,i,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = i;   (*disc)[*len - 1].y = y;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len-1].x = -1;
			break;
		    }
		}
		break;
	    case U:
		for(int i=p[item].y;i>=0;i--){
		    if(board[i][x] == turn)
			break;
		    if(board[i][x] == 0){
			if(uniqueItem(i,x,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = x;   (*disc)[*len - 1].y = i;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len-1].x = -1;
			break;
		    }
		}
		break;
	    case D:
		for(int i=p[item].y;i<8;i++){
		    if(board[i][x] == turn)
			break;
		    if(board[i][x] == 0){
			if(uniqueItem(i,x,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = x;   (*disc)[*len - 1].y = i;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len-1].x = -1;
			break;
		    }
		}
		break;
	    case LD:
		for(int i=p[item].x,j=p[item].y;i>=0 && j<8;i--,j++){
		    if(board[j][i] == turn)
			break;
		    if(board[j][i] == 0){
			if(uniqueItem(j,i,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = i;	(*disc)[*len - 1].y = j;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len - 1].x = -1;
			break;
		    }
		}
		break;
	    case RD:
		for(int i=p[item].x,j=p[item].y;i<8 && j<8;i++,j++){
		    if(board[j][i] == turn)
			break;
		    if(board[j][i] == 0){
			if(uniqueItem(j,i,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = i;	(*disc)[*len - 1].y = j;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len - 1].x = -1;
			break;
		    }
		}
		break;
	    case RU:
		for(int i=p[item].x,j=p[item].y;i<8 && j>=0;i++,j--){
		    if(board[j][i] == turn)
			break;
		    if(board[j][i] == 0){
			if(uniqueItem(j,i,*disc,*len)==0)
			    break;
			(*disc)[*len - 1].x = i;	(*disc)[*len - 1].y = j;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len - 1].x = -1;
			break;
		    }
		}
		break;
	    case LU: 
		for(int i=p[item].x,j=p[item].y;i>=0 && j>=0;i--,j--){
		    if(board[j][i] == turn)
			break;
		    if(board[j][i] == 0){
			if(uniqueItem(j,i,*disc,*len)==0)
			    break;
			
			(*disc)[*len - 1].x = i;	(*disc)[*len - 1].y = j;
			(*len)++;
			*disc = realloc(*disc,sizeof(pos)*(*len));
			(*disc)[*len - 1].x = -1;
			break;
		    }
	    
		}
		break;
	}
    }
    free(p);
}

int checkValid(int y,int x,pos* p){
    for(int item =0;p[item].x!=-1;item++){
	if(p[item].x == x && p[item].y == y)
	    return 1;
    }
    return 0;
}

int goLeft(int y,int x,int turn){
     for(int i=x-1;i>=0;i--){
	if(board[y][i] == turn)
	    return 1;
     }
     return 0;
}
int goRight(int y,int x,int turn){
     for(int i=x+1;i<8;i++){
	if(board[y][i] == turn)
	    return 1;
     }
     return 0;
}

int goDown(int y,int x,int turn){
    for(int i=y+1;i<8;i++){
	if(board[i][x] == turn)
	    return 1;
    }
    return 0;
}

int goUp(int y,int x,int turn){
    for(int i=y-1;i>=0;i--){
	if(board[i][x] == turn)
	    return 1;
    }
    return 0;
}

int goRightDown(int y,int x,int turn){
    for(int i=y+1,j=x+1;i<8 && j<8;j++,i++){
	if(board[i][j] == turn)
	    return 1;
    }
    return 0;
}

int goRightUp(int y,int x,int turn){
    for(int i=y-1,j=x+1;j<8 && i>=0;j++,i--){
	if(board[i][j] == turn)
	    return 1;
    }
    return 0;
}

int goLeftUp(int y,int x,int turn){
    for(int i=y-1,j=x-1;i>=0&&j>=0;i--,j--){
	if(board[i][j] == turn)
	    return 1;
    }
    return 0;
}

int goLeftDown(int y,int x,int turn){
    for(int i=y+1,j=x-1;i<8&&j>=0;j--,i++){
	if(board[i][j] == turn)
	    return 1;
    }
    return 0;
}

void printMoves(pos* p,int turn){
    printf("\nplayer '%c':\n",turn == 2 ? 'O':'X');
    for(int i=0;p[i].x != -1;i++)
	printf("(%d,%d)\t",p[i].x+1,p[i].y+1);
    printf("\n");
}

int admit(int y ,int x ,int turn ,pos* pm ,int *counter,int board[][8]){
    if(!checkValid(y,x,pm) ){
	return 0;
    }
    board[y][x] = turn;
    counter[turn-1]++;
    if(goRight(y,x,turn))
	for(int i=x+1;i<8;i++){//RIGHT
	    if(board[y][i] == (turn == Player_X ? Player_O : Player_X)){
		board[y][i] = turn;
		counter[turn-1]++;
		counter[turn == Player_X ? 1 : 0]--;
	    }else if(board[y][i] == 0 || board[y][i] == turn)
		break;
	}
    if(goLeft(y,x,turn))
	for(int i=x-1;i>=0;i--){ //LEFT
	    if(board[y][i] == (turn == Player_X ? Player_O : Player_X)){
		board[y][i] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X)?1:0]--;
	    }else if(board[y][i] == 0 || board[y][i] == turn)
		break;
	}
    if(goUp(y,x,turn))
	for(int i=y-1;i>=0;i--){//UP
	    if(board[i][x] == (turn == Player_X ? Player_O : Player_X)){
		board[i][x] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X) ? 1:0]--;
	    }else if(board[i][x] == 0 || board[i][x] == turn)
		break;
	}
    if(goDown(y,x,turn))
	for(int i=y+1;i<8;i++){//DOWN
	    if(board[i][x] == (turn == Player_X ? Player_O : Player_X)){
		board[i][x] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X)?1:0]--;
	    }else if(board[i][x] == 0 || board[i][x] == turn)
		break;
	}
    if(goRightDown(y,x,turn))
	for(int i=y+1,j=x+1;i<8 && j<8;j++,i++){//RD
	    if(board[i][j] == (turn==Player_X?Player_O:Player_X)){
		board[i][j] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X)?1:0]--;
	    }else if(board[i][j] == 0 || board[i][j] == turn)
		break;
	}
    if(goRightUp(y,x,turn))
	for(int i=y-1,j=x+1;j<8 && i>=0;j++,i--){ // RU
	    if(board[i][j] == (turn == Player_X ? Player_O : Player_X)){
		board[i][j] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X)?1:0]--;
	    }else if(board[i][j] == 0 || board[i][j] == turn)
		break;
	}
    if(goLeftDown(y,x,turn))
	for(int i=y+1,j=x-1;i<8 && j>=0;j--,i++){ // LD
	    if(board[i][j] == (turn == Player_X ? Player_O : Player_X)){
		board[i][j] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X)?1:0]--;
	    }else if(board[i][j] == 0 || board[i][j] == turn)
		break;
	}
    if(goLeftUp(y,x,turn))
	for(int i=y-1,j=x-1;i>=0&&j>=0;i--,j--){ // LU
	    if(board[i][j] == (turn == Player_X ? Player_O : Player_X)){
		board[i][j] = turn;
		counter[turn-1]++;
		counter[(turn==Player_X)?1:0]--;
	    }else if(board[i][j] == 0 || board[i][j] == turn)
		break;
	}
    return 1;
}

int uniqueItem(int newItem_Y,int newItem_X,pos* arr,int len){
    for(int i=0;i<len;i++){
	if(newItem_X == arr[i].x && newItem_Y == arr[i].y)
	    return 0;
    }
    return 1;
}

void compMove(pos* pm,int *disc_counter){
    if(pm[0].x==-1){
	printf("no move\n");
	return;
    }
    int copy_counter[2]; 
    int copyBoard[8][8];	
    int bestChoice = 0;
    pos bestMove = pm[0];

    for(int i=0;pm[i].x!=-1;i++){

	for(int i=0;i<8;i++)
	    for(int j=0;j<8;j++)              // reset the `copyBoard` with current discs
		copyBoard[i][j] = board[i][j];

	copy_counter[0] = disc_counter[0];                 // reset the 
	copy_counter[1] = disc_counter[1];                 // copy counter

	admit(pm[i].y,pm[i].x,Player_X,pm,copy_counter,copyBoard);
	if(bestChoice < copy_counter[0]){
	    bestChoice = copy_counter[0];
	    bestMove = pm[i];
	}
    }
    admit(bestMove.y,bestMove.x,Player_X,pm,disc_counter,board);
}

void options(void){
    printf("=======================OPTIONS=======================\n");
    printf("1)Start Game\n");
    printf("2)show latest game\n");
    printf("3)Exit\n");
}



void history(char perm,int turn){
    if(perm == 'w'){
	FILE* fp = fopen("history","w");
	fprintf(fp,"turn: %d\n\n",turn);
	for(int row=0;row<8;row++){
	    if(!row){
		fputs("  ",fp);
		for(int k=1;k<=8;k++)
			fprintf(fp,"\033[90m%d \033[0m",k);
		fprintf(fp,"\n");
	    }

	    fprintf(fp,"\033[90m%d \033[0m",row+1);
	    for(int column=0;column<8;column++){
		switch(board[row][column]){
		    case 0:
			fprintf(fp,"\033[33m- \033[0m");
			break;
		    case 1:
			fprintf(fp,"\033[31mX \033[0m");
			break;
		    case 2:
			fprintf(fp,"\033[36mO \033[0m");
			break;
		}
	    }
	    fprintf(fp,"\n");
	}
	fclose(fp);
	
    }else if(perm == 'r'){
	FILE* fp = fopen("history","r");
	if(fp == NULL){
	    printf("You haven't played a game.\n");
	    return;
	}
	char line[10];
	while(fgets(line,10,fp)!=NULL){
	    printf("%s",line);
	}
	fclose(fp);
    }
}

void setupTerminal(struct termios* tty) {
    tcgetattr(STDIN_FILENO, tty);
    tty->c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, tty);
    printf("\033[?25l");
}

void restoreTerminal(struct termios* tty) {
    tcsetattr(STDIN_FILENO, TCSANOW, tty);
    printf("\033[?25h");
}

void clearScreen(void){
	printf("\033[2J\033[H");
}

int playLatestGame(int *counter){
    
    FILE* fp = fopen("history","r");
    if(fp == NULL)
	return 2;

    for(int k=0;k<6;k++)getc(fp);
    int turn = getc(fp) - 48;
    int i=0,j=0;
    char ch;
    counter[0] = 0; 	counter[1]=0;

    while((ch = getc(fp)) != EOF){
	if(j>=8){
		i++;
		j=0;
	}
	if(ch == '-' ){
		board[i][j] = 0;
		j++;
	}else if( ch == 'X' ){
		board[i][j] = 1;
		counter[0]++;
		j++;
	}else if(ch == 'O'){
		board[i][j] = 2;
		counter[1]++;
		j++;
	}
    } 
    return turn;
}

int win(void){
    pos *p = find(1);
    if(p[0].x == -1){
	free(p);
	p = find(2);
	if(p[0].x==-1){
	    free(p);
	    return 1;
	}
    }
    free(p);
    return 0;
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

void drawCursor(pos* cursor,int turn){
    printf("%d %d\n",cursor->x/2,cursor->y-6);
    printf("\e[%d;%dH%c",cursor->y,cursor->x,
	    turn == Player_X ? 'X':'O'
	  );
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


void gameSetup(char *isBot, int *key){
    clearScreen();
    printf("bot? y(es) , n(o)\n");
    scanf(" %c", isBot);

    clearScreen();
    printf("wanna play previous game? (1 or 0): ");
    *key = getchar() - 48;
    fflush(stdin);
}

void handleInput(int disc_counter[],int* key, enum Player turn, pos* cursor, pos* p){

    if(p[0].x == -1){ // check if there is possible move
	turn = turn == Player_X ? Player_O : Player_X;
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
    if(!admit(cursor->y-7,cursor->x/2-1,turn,p,disc_counter,board)){
	printf("\e[20;10HNot valid Disc!!\n");
	usleep(1000*1000);
	*key = 0;
	handleInput(disc_counter,key,turn,cursor,p);
    }
    board[cursor->y - 7][cursor->x / 2 - 1] = turn;
    ////*turn = *turn == Player_X ? Player_O : Player_X;
}

void displayGameResult(int disc_counter[]){
    printf("number of X: %d\nnumber of O: %d\n\n",disc_counter[0],disc_counter[1]);
    if(disc_counter[0]==disc_counter[1]){
	printf("ended in draw :|\n");
	return;
    }
    printf("winner: %c\n",(disc_counter[0]>disc_counter[1] ? 'X':'O'));
}



void runGame(void){
    pos* p;
    int disc_counter[2] = {2,2}; // 0=>X, 1=>O
    char isBot;
    int key;

    gameSetup(&isBot,&key);
    
    enum Player turn = (key != 0 ? playLatestGame(disc_counter) : Player_O); // make the turn for previous match

    pos cursor = {.y = 7, .x=3};

    while(1){
	clearScreen();
	if(win() == 1){
	    displayGameResult(disc_counter);
	    break;
	}
	
	if(isBot == 'y'){
	    p = find(turn);
	    if(turn == Player_O){
		handleInput(disc_counter,&key, turn, &cursor, p);
		free(p);
		key = 0;
	    }else {
		clearScreen();
		drawBoard();
		compMove(p,disc_counter);
		usleep(1500*1000);

		free(p);

	    }
	}else if(isBot == 'n'){
	    p = find(turn);
	    handleInput(disc_counter,&key, turn, &cursor, p);
	    free(p);
	    key = 0;
	    usleep(100*1000);
	}
	turn = turn == Player_X ? Player_O : Player_X;
	history('w',turn);
    }
}


void initializeBoard(void){
    for(int i=0;i<8;i++)
	for(int j=0;j<8;j++)
	    board[i][j] = 0;
    board[3][3] = 1;	board[3][4] = 2;	board[4][3] = 2;	board[4][4] = 1;
}
