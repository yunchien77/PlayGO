#include "chessData.h"
#include "./ts/minmax.c"
#include<time.h>
int step;
void writeChessBoard(Chess* chess, int player, int* x, int* y) {
    int first=1;
    printf("be call");
    
    while(1){
        if(chess->size==0&&first) {initial_table();put_chess(11,11,BLACK);*x=*y=11;}
        else if(chess->size==1&&first){
            initial_table();
            put_chess(11,11,BLACK);
            put_chess(10,10,WHITE);
            *x=*y=10;
        }
        else if(first){
            put_chess(chess->cord[chess->size-1].y,chess->cord[chess->size-1].x,player==0?WHITE:BLACK);
            clock_t st=clock();
            printf("|%d|",minmax(Depth,-win_score,win_score,1,player==0?BLACK:WHITE));
            printf("time:%.3f\n",(double)(clock()-st)/1000);
            put_chess(toY(step),toX(step),player==0?BLACK:WHITE);
            *x=toX(step);*y=toY(step);
        }
        first=0;
        if(setXY(chess,*x,*y,player)==1){
                return;    
        }
    }
}

