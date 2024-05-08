#include "minmax.c"
#include "item.h"
#include<stdio.h>
#include<time.h>
char map[N][N];
int step,step2;
int main(){
    initial_table();
    // put_chess(11,11,BLACK);
    // put_chess(9,11,WHITE);
    // put_chess(9,9,WHITE);
    // put_chess(11,12,BLACK);
    int cnt=1;
    int sg=0;
    clock_t start;
    int lastpos[N*N],*p=lastpos;
    int who,priority;
    printf("(Black:1,White:2):");
    scanf("%d",&who);
    printf("(first:1,second:2)");
    scanf("%d",&priority);

    while(1){
        if(cnt==1&&priority==2) put_chess(N/2,N/2,who==1?WHITE:BLACK);
        if(cnt++!=1){

            if(unreal_long_search(Bigger_Unreal_Depth,who==1?WHITE:BLACK))
                step=step2;
            else
                printf("| %d |\n",sg=minmax(Depth,-win_score,win_score,1,who==1?WHITE:BLACK));
            printf("%d %d\n",toY(step),toX(step));
            printf("\ntime:%.6f\n",(double)((clock()-start))/CLOCKS_PER_SEC);
            put_chess(toY(step),toX(step),who==1?WHITE:BLACK);
            *p++=step;
        }
        //chess
        for(int i=0;i<N;i++){
            printf("%2d| ",i);
            for(int j=0;j<N;j++){
                if(map[i][j])
                    printf("%d ",map[i][j]);
                else printf("* ");
            }
            printf("\n");
        }
        printf("   ");
        for(int i=0;i<N;i++)
            printf("%2d",i%10);
        printf("\n");
        if(sg==__INT32_MAX__) break;
        //input
        int ty,tx,Flag=0;
        do{
            Flag=0;
            printf("Next(Y,X):");scanf("%d %d",&ty,&tx);
            if(ty==-1&&tx==-1){
                p--;
                remove_chess(toY(*p),toX(*p));
                p--;
                remove_chess(toY(*p),toX(*p));
                for(int i=0;i<N;i++){
                    printf("%2d| ",i);
                    for(int j=0;j<N;j++){
                        if(map[i][j])
                            printf("%d ",map[i][j]);
                        else printf("* ");
                    }
                    printf("\n");
                }
                printf("   ");
                for(int i=0;i<N;i++)
                    printf("%2d",i%10);
                printf("\n");
                Flag=1;
            }
        }while(map[ty][tx]||Flag);
        *p++=toPos(ty,tx);
        start=clock();
        put_chess(ty,tx,who==2?WHITE:BLACK);
    }
    printf("\ngame over\n");
    char chekc;
    scanf(" %c",&chekc);
    free_big_map();
}