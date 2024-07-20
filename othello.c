#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
// ======================================MARCOS/VARABLE/STRUCTURES

typedef struct pos{
	int x,y;
} pos;

int board[8][8] = {{0}};

#ifdef _MSC_VER     		// For Microsoft Visual C++ compiler, which does not support \e
	#define ESC "\e"	// Use th standard hexadecimal representation
#else				// For compilers that might support \e as a non-standard extension
	#define ESC "\x1B"  	// Use the escape sequence if supported
#endif

#define R 1
#define L 2 
#define U 3
#define D 4
#define RU 13
#define RD 14
#define LU 23
#define LD 24

// ======================================FUNCTION_PROTOTYPES

void draw(void); /* to draw the board */

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

void clearScreen(void); /*clear screen with ansi codes*/

int playLatestGame(int *); /*to play the latest game*/

int win(void); /*check if one win*/
// ======================================MAIN_FUNCTION
int main(){
	board[3][3] = 1;	board[3][4] = 2;	board[4][3] = 2;	board[4][4] = 1;

	int key;

	while(1){
		
		options();
		scanf("%d",&key);

		switch(key){
			case 1: //game
				clearScreen();
				printf("===============================WELCOME===============================\n\n");
				
				pos* p;
				pos input;
				int counter[2] = {2,2}; // 0=>X, 1=>O
				char isBot;
				
				printf("bot? y(es) , n(o)\n");
				scanf(" %c",&isBot);

				clearScreen();
				printf("wanna play previous game? (1 or 0): ");
				scanf(" %d",&key);
				
				int turn = (key!=0 ? playLatestGame(counter) : 2); // make the turn for previous match
				while(1){
					clearScreen();
					if(win()==1){
						printf("number of X: %d\nnumber of O: %d\n\n",counter[0],counter[1]);
						if(counter[0]==counter[1]){
							printf("ended in draw :|\n");
							break;
						}
						printf("winner: %c\n",(counter[0]>counter[1] ? 'X':'O'));
						break;
					}
					printf("\nnumber of X: %d\nnumber of O: %d\n\n",counter[0],counter[1]);
					draw();
					
					if(isBot == 'y'){
						if(turn == 2){
							p = find(turn);
							printMoves(p,turn);
							if(p[0].x != -1){ // check if there is possible move
								scanf(" %d %d",&input.x,&input.y);
								if(!admit(input.y-1,input.x-1,turn,p,counter,board)){
									printf("Not valid Disc!!\n");
									usleep(1000*1000);
									free(p);
									continue;
								}
							}
							free(p);
							turn = 1;
						}else {
							printf("%s[2J%s[H",ESC,ESC);
							draw();
							p = find(turn);
							printMoves(p,turn);
							compMove(p,counter);
							usleep(1500*1000);
			
							
							free(p);
							turn = 2;
	
						}
					}else if(isBot == 'n'){
						p = find(turn);
						printMoves(p,turn);
						scanf(" %d %d",&input.x,&input.y);
						if(turn == 2){
							if(!admit(input.y-1,input.x-1,turn,p,counter,board)){
								printf("Not valid Disc!!\n");
								usleep(1000*1000);
								free(p);
								continue;
							}
							free(p);
							turn = 1;
						}else{
							if(!admit(input.y-1,input.x-1,turn,p,counter,board)){
								printf("Not valid Disc!!\n");
								usleep(1000*1000);
								free(p);
								continue;
							}
							free(p);
							turn = 2;
						}
						usleep(100*1000);
					}
					history('w',turn);
				}
				break;
			case 2: //latest game
				clearScreen();
				history('r',0); // 0 here do nothing 
				break;
		}	
	}
}
// ======================================FUNCTION_DEFINTITION
void draw(void){
	for(int row=0;row<8;row++){
		if(!row){
			printf("  ");
			for(int k=1;k<=8;k++)
				printf("%s[90m%d %s[0m",ESC,k,ESC);
			printf("\n");
		}

		printf("%s[90m%d %s[0m",ESC,row+1,ESC);
		for(int column=0;column<8;column++){
			switch(board[row][column]){
				case 0:
					printf("%s[33m- %s[0m",ESC,ESC);
					break;
				case 1:
					printf("%s[31mX %s[0m",ESC,ESC);
					break;
				case 2:
					printf("%s[36mO %s[0m",ESC,ESC);
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
			if(board[y][i] == (turn==1?2:1)){
				board[y][i] = turn;
				counter[turn-1]++;
				counter[turn==1?1:0]--;
			}else if(board[y][i] == 0 || board[y][i] == turn)
				break;
		}
	if(goLeft(y,x,turn))
		for(int i=x-1;i>=0;i--){ //LEFT
			if(board[y][i] == (turn==1?2:1)){
				board[y][i] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
			}else if(board[y][i] == 0 || board[y][i] == turn)
				break;
		}
	if(goUp(y,x,turn))
		for(int i=y-1;i>=0;i--){//UP
			if(board[i][x] == (turn==1?2:1)){
				board[i][x] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
			}else if(board[i][x] == 0 || board[i][x] == turn)
				break;
		}
	if(goDown(y,x,turn))
		for(int i=y+1;i<8;i++){//DOWN
			if(board[i][x] == (turn==1?2:1)){
				board[i][x] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
			}else if(board[i][x] == 0 || board[i][x] == turn)
				break;
		}
	if(goRightDown(y,x,turn))
		for(int i=y+1,j=x+1;i<8 && j<8;j++,i++){//RD
			if(board[i][j] == (turn==1?2:1)){
				board[i][j] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
			}else if(board[i][j] == 0 || board[i][j] == turn)
				break;
		}
	if(goRightUp(y,x,turn))
		for(int i=y-1,j=x+1;j<8 && i>=0;j++,i--){ // RU
			if(board[i][j] == (turn==1?2:1)){
				board[i][j] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
			}else if(board[i][j] == 0 || board[i][j] == turn)
				break;
		}
	if(goLeftDown(y,x,turn))
		for(int i=y+1,j=x-1;i<8&&j>=0;j--,i++){ // LD
			if(board[i][j] == (turn==1?2:1)){
				board[i][j] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
			}else if(board[i][j] == 0 || board[i][j] == turn)
				break;
		}
	if(goLeftUp(y,x,turn))
		for(int i=y-1,j=x-1;i>=0&&j>=0;i--,j--){ // LU
			if(board[i][j] == (turn==1?2:1)){
				board[i][j] = turn;
				counter[turn-1]++;
				counter[(turn==1)?1:0]--;
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

void compMove(pos* pm,int *counter){
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

		copy_counter[0] = counter[0];                 // reset the 
		copy_counter[1] = counter[1];                 // copy counter

		admit(pm[i].y,pm[i].x,1,pm,copy_counter,copyBoard);
		if(bestChoice < copy_counter[0]){
			bestChoice = copy_counter[0];
			bestMove = pm[i];
		}
	}
	admit(bestMove.y,bestMove.x,1,pm,counter,board);
}

void options(void){
	printf("=======================OPTIONS=======================\n");
	printf("1)Start Game\n");
	printf("2)show latest game\n");
}



void history(char job,int turn){
	if(job == 'w'){
		FILE* fp = fopen("history","w");
		fprintf(fp,"turn: %d\n\n",turn);
		for(int row=0;row<8;row++){
			if(!row){
				fputs("  ",fp);
				for(int k=1;k<=8;k++)
					fprintf(fp,"%s[90m%d %s[0m",ESC,k,ESC);
				fprintf(fp,"\n");
			}

			fprintf(fp,"%s[90m%d %s[0m",ESC,row+1,ESC);
			for(int column=0;column<8;column++){
				switch(board[row][column]){
					case 0:
						fprintf(fp,"%s[33m- %s[0m",ESC,ESC);
						//fputs("- ",fp);
						break;
					case 1:
						fprintf(fp,"%s[31mX %s[0m",ESC,ESC);
						//fputs("X ",fp);
						break;
					case 2:
						fprintf(fp,"%s[36mO %s[0m",ESC,ESC);
						//fputs("O ",fp);
						break;
				}
			}
			fprintf(fp,"\n");
		}
		fclose(fp);
		
	}else if(job == 'r'){
		FILE* fp = fopen("history","r");
		char line[10];
		while(fgets(line,10,fp)!=NULL){
			printf("%s",line);
		}
		fclose(fp);
	}
}

void clearScreen(void){
	printf("%s[2J%s[H",ESC,ESC);
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
