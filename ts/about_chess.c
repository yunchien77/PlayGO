#include"item.h"
#include "Transposition_Table.c"
char map[N][N],relate_map[N][N],cnt_map[N][N];
int pre_i_chess,pre_j_chess;
void put_chess(const int y,const int x,const char role){
    // if(!map[y][x]){
        map[y][x]=role;
        updata_key(y,x,role);
        updata_score(y,x,BLACK);
        updata_score(y,x,WHITE);
    // }

    
    for(pre_i_chess=y-range;pre_i_chess<=y+range;pre_i_chess++){
        for(pre_j_chess=x-range;pre_j_chess<=x+range;pre_j_chess++){
            if(pre_i_chess>>31||pre_j_chess>>31||pre_i_chess>=N||pre_j_chess>=N) continue;
            cnt_map[pre_i_chess][pre_j_chess]++;
            relate_map[pre_i_chess][pre_j_chess]=!map[pre_i_chess][pre_j_chess];
        }
    }
}
void remove_chess(const int y,const int x){
    // if(map[y][x]){
        updata_key(y,x,map[y][x]);
        map[y][x]=0;
        updata_score(y,x,BLACK);
        updata_score(y,x,WHITE);
    // }



    for(pre_i_chess=y-range;pre_i_chess<=y+range;pre_i_chess++){
        for(pre_j_chess=x-range;pre_j_chess<=x+range;pre_j_chess++){
            if(pre_i_chess>>31||pre_j_chess>>31||pre_i_chess>=N||pre_j_chess>=N) continue;
            cnt_map[pre_i_chess][pre_j_chess]--;
            if(!cnt_map[pre_i_chess][pre_j_chess])
                relate_map[pre_i_chess][pre_j_chess]=0;
        }
    }
    relate_map[y][x]=1;
}