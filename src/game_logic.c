#include "../include/game_logic.h"
pos* findOpposite(int y,int x){
    size_t len = 1;
    int turn = board[y][x];
    pos* p = (pos*) malloc(1*sizeof(pos));
    p[0].x = -1;
    for(int j=y-1;j<y+2 ;j++){
        if(j<0 || j>=BOARD_SIZE)
            continue;
        for(int i=x-1;i<x+2;i++){
            if(i<0 || i>=BOARD_SIZE)
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
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            if(board[i][j] == turn)
                possibleMove(i,j,&tmp,&len);
        }
    }
    printf("\n");
    return tmp;
}
int confirm(int y ,int x ,int turn ,pos* pm ,int *counter,int board[][BOARD_SIZE]){
    if(!checkValid(y,x,pm) ){
        return 0;
    }
    board[y][x] = turn;
    counter[turn-1]++;
    if(goRight(y,x,turn))
        for(int i=x+1;i<BOARD_SIZE;i++){//RIGHT
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
        for(int i=y+1;i<BOARD_SIZE;i++){//DOWN
            if(board[i][x] == (turn == Player_X ? Player_O : Player_X)){
                board[i][x] = turn;
                counter[turn-1]++;
                counter[(turn==Player_X)?1:0]--;
            }else if(board[i][x] == 0 || board[i][x] == turn)
                break;
        }
    if(goRightDown(y,x,turn))
        for(int i=y+1,j=x+1;i<BOARD_SIZE && j<BOARD_SIZE;j++,i++){//RD
            if(board[i][j] == (turn==Player_X?Player_O:Player_X)){
                board[i][j] = turn;
                counter[turn-1]++;
                counter[(turn==Player_X)?1:0]--;
            }else if(board[i][j] == 0 || board[i][j] == turn)
                break;
        }
    if(goRightUp(y,x,turn))
        for(int i=y-1,j=x+1;j<BOARD_SIZE && i>=0;j++,i--){ // RU
            if(board[i][j] == (turn == Player_X ? Player_O : Player_X)){
                board[i][j] = turn;
                counter[turn-1]++;
                counter[(turn==Player_X)?1:0]--;
            }else if(board[i][j] == 0 || board[i][j] == turn)
                break;
        }
    if(goLeftDown(y,x,turn))
        for(int i=y+1,j=x-1;i<BOARD_SIZE && j>=0;j--,i++){ // LD
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
                for(int i=p[item].x;i<BOARD_SIZE;i++){
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
                for(int i=p[item].y;i<BOARD_SIZE;i++){
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
                for(int i=p[item].x,j=p[item].y;i>=0 && j<BOARD_SIZE;i--,j++){
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
                for(int i=p[item].x,j=p[item].y;i<BOARD_SIZE && j<BOARD_SIZE;i++,j++){
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
                for(int i=p[item].x,j=p[item].y;i<BOARD_SIZE && j>=0;i++,j--){
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
    for(int i=x+1;i<BOARD_SIZE;i++){
        if(board[y][i] == turn)
            return 1;
    }
    return 0;
}
int goDown(int y,int x,int turn){
    for(int i=y+1;i<BOARD_SIZE;i++){
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
    for(int i=y+1,j=x+1;i<BOARD_SIZE && j<BOARD_SIZE;j++,i++){
        if(board[i][j] == turn)
            return 1;
    }
    return 0;
}
int goRightUp(int y,int x,int turn){
    for(int i=y-1,j=x+1;j<BOARD_SIZE && i>=0;j++,i--){
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
    for(int i=y+1,j=x-1;i<BOARD_SIZE&&j>=0;j--,i++){
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
int uniqueItem(int newItem_Y,int newItem_X,pos* arr,int len){
    for(int i=0;i<len;i++){
        if(newItem_X == arr[i].x && newItem_Y == arr[i].y)
            return 0;
    }
    return 1;
}