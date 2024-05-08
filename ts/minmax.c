#include "getscore.c"
#include "GetAvailable_Move.c"
char map[N][N];
int step,step2;
char factory(const char role,const char deep);
int Is_Game_End(const char role,const int y,const int x){
    if(role==BLACK){
        if(score_map[y][x].Black_total==one_five){
            return 1;
        }else if(score_map[y][x].Black_total==three_four){
            for(int i=0;i<N;i++)
                for(int j=0;j<N;j++)
                    if(score_map[i][j].White_total==one_five) return 0;
            for(int dir=0;dir<4;dir++){
                if(score_map[y][x].Black_line[dir].DFour_count||score_map[y][x].Black_line[dir].Jfour_count||score_map[y][x].Black_line[dir].Three_count){
                    for(int reve=0;reve<2;reve++){
                        for(int u=1;u<=4;u++){
                            int next_y=y+u*move_y[dir]*move_dir[reve],
                                next_x=x+u*move_x[dir]*move_dir[reve];
                            if(next_y>>31||next_y>=N||next_x>>31||next_x>=N) continue;
                            if(score_map[next_y][next_x].Black_line[dir].DFour_count||score_map[next_y][next_x].Black_line[dir].Jfour_count||score_map[next_y][next_x].Black_line[dir].Three_count){
                                for(int dir2=0;dir2<4;dir2++){
                                    if(score_map[next_y][next_x].White_line[dir2].DFour_count||score_map[next_y][next_x].White_line[dir2].Four_count||score_map[next_y][next_x].White_line[dir2].Jfour_count){
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return 1;
        }
    }else{
        if(score_map[y][x].White_total==one_five){
            return 1;
        }else if(score_map[y][x].White_total==three_four){
            for(int i=0;i<N;i++)
                for(int j=0;j<N;j++)
                    if(score_map[i][j].Black_total==one_five) return 0;
            for(int dir=0;dir<4;dir++){
                if(score_map[y][x].White_line[dir].DFour_count||score_map[y][x].White_line[dir].Jfour_count||score_map[y][x].White_line[dir].Three_count){
                    for(int reve=0;reve<2;reve++){
                        for(int u=1;u<=4;u++){
                            int next_y=y+u*move_y[dir]*move_dir[reve],
                                next_x=x+u*move_x[dir]*move_dir[reve];
                            if(next_y>>31||next_y>=N||next_x>>31||next_x>=N) continue;
                            if(score_map[next_y][next_x].White_line[dir].DFour_count||score_map[next_y][next_x].White_line[dir].Jfour_count||score_map[next_y][next_x].White_line[dir].Three_count){
                                for(int dir2=0;dir2<4;dir2++){
                                    if(score_map[next_y][next_x].Black_line[dir2].DFour_count||score_map[next_y][next_x].Black_line[dir2].Four_count||score_map[next_y][next_x].Black_line[dir2].Jfour_count){
                                        return 0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return 1;      
        }
    }
    return 0;
}

// int reco[N],*pr=reco,reco_deep[N],*pr2=reco_deep;
int CM[Depth+1][N*N],CM2[Bigger_Unreal_Depth+1][N*N];
int minmax(const signed char deep,int alpha,const int beta,const char is_first,const char role){
    int value;
    GetAvailable_Move(&CM[deep][0],role);
    if(!deep) {
        if(factory(role,Root_Unreal_Depth)) value=win_score;
        else 
            value=Count_role_score(role);
        updata_value(value,deep+1,EXACT);
        return value;
    }
    if(deep==1) if(factory(role,Root_Unreal_Depth)) {updata_value(win_score,deep+1,EXACT);return win_score;}
    
    char type,tmpdeep,total_type=EXACT;
    int best_value=1<<31;
    
    if(is_first && CM[deep][1]==-1){step=CM[deep][0];return 0;}
    if(deep==Depth-1||deep==Depth){
        if(factory(role,Bigger_Unreal_Depth)) {if(deep==Depth) step=step2;updata_value(win_score,deep+1,EXACT);return win_score;}
    }
    int index=0,best_step;
                // if(deep>=4){
                //     printf("deep(%d)",deep);
                //     for(int i=0;Can_Move[i]!=-1;i++){
                //         printf("(%d,%d)",toY(Can_Move[i]),toX(Can_Move[i]));
                // }
                // printf("\n");}
    while(CM[deep][index]!=-1){//check all son
            // if(deep>=3)
            //     printf("(%d %d %d):",toY(CM[deep][index]),toX(CM[deep][index]),role);
        
        if(Is_Game_End(role,toY(CM[deep][index]),toX(CM[deep][index]))){
            //五 或 四三
            value=win_score;
        }else{
            //放旗子
                    //printf("put(%d,%d)",toY(Can_Move[index]),toX(Can_Move[index]));
            put_chess(toY(CM[deep][index]),toX(CM[deep][index]),role);
                // *pr++=CM[deep][index];*pr2++=deep;
            //成功取值  && (深度相等 && (精準 || value>=beta) ||  勝利分 /*||深度大於*/) ->不計算
            //!成功取值  || ((!深度相等 || (!精準 && value<beta)) && ( !勝利分)) ->計算
            if(!try_get_value(&value,&tmpdeep,&type)||abs(value)!=win_score&&(tmpdeep!=deep||(!type&&value<beta)))
                value=-minmax(deep-1,-beta,-alpha,0,GetOppo(role));
            total_type&=get_type();
            //收旗子
                    // printf("rem(%d,%d)",toY(Can_Move[index]),toX(Can_Move[index]));
                    // if(deep==Depth) printf("\n");
                // *--pr;*--pr2;
             remove_chess(toY(CM[deep][index]),toX(CM[deep][index]));
        }
                // if(reco[0]==toPos(6,6)&&reco[1]==toPos(6,7)&&reco[2]==toPos(5,7)&&deep==3||reco[0]==toPos(5,7)&&deep==Depth){
                //     for(int i=0;reco+i<=pr;i++){
                //         printf("(%d %d,%d)",toY(reco[i]),toX(reco[i]),reco_deep[i]);
                //     }
                //     printf("v:%d a:%d b:%d best:%d key%llu\n",value,alpha,beta,best_value,The_Key);
                // }
        if(value>best_value){
            // if(abs(value)!=win_score) value+=20;
            best_value=value;
            best_step=CM[deep][index];
        }
        if(value>alpha) alpha=value;
        if(alpha>=beta) {
            total_type=!EXACT;
            break;
        }
        index++;
    }
    if(is_first){
        step=best_step;
    }

    updata_value(best_value,deep+1,total_type);
        // printf("(a:%d)\n",alpha);
    return alpha;
}


char unreal_search(const char deep,const char role,const char first){
    if(!deep) {
        updata_value_v(0,1,0,role);
        return 0;
    }
    Unreal_Get_Move(&CM2[deep][0],role,!(deep&1));

        // printf("deep(%d,role%d):",deep,role);
        // for(int i=0;CM2[deep][i]!=-1;i++){
        //     printf("(%d,%d)",toY(CM2[deep][i]),toX(CM2[deep][i]));
        // }
        // printf("\n");
        // int wait;
        // scanf("%d",&wait);
    if(CM2[deep][0]==-1){
        if(deep&1) {
            updata_value_v(0,deep+1,EXACT,role);
            return 0;
        }
        else {
            updata_value_v(1,deep+1,EXACT,role);
            return 1;
        }
    }

    char success,value,total_type=EXACT;
    if(deep&1){
        //or=
        success=0;
        for(int index=0;CM2[deep][index]!=-1&&!success;index++){
            if(Is_Game_End(role,toY(CM2[deep][index]),toX(CM2[deep][index]))){
                updata_value_v(1,deep+1,EXACT,role);
                if(first) step2=CM2[deep][index];
                return 1;
            }
            put_chess(toY(CM2[deep][index]),toX(CM2[deep][index]),role);
            if(try_get_value_v(&value,deep,role))
                success|=value;
            else
                success|=unreal_search(deep-1,GetOppo(role),0);
            if(success&&first) step2=CM2[deep][index];
            total_type&=get_type_v();
            remove_chess(toY(CM2[deep][index]),toX(CM2[deep][index]));
        }
    }else{
        //and=
        success=1;
        for(int index=0;CM2[deep][index]!=-1&&success;index++){
            if(Is_Game_End(role,toY(CM2[deep][index]),toX(CM2[deep][index]))){
                updata_value_v(0,deep+1,EXACT,role);
                return 0;
            }

            put_chess(toY(CM2[deep][index]),toX(CM2[deep][index]),role);
            if(try_get_value_v(&value,deep,role))
                success&=value;              
            else
                success&=unreal_search(deep-1,GetOppo(role),0);
            total_type&=get_type_v();
            remove_chess(toY(CM2[deep][index]),toX(CM2[deep][index]));
        }
    }

    updata_value_v(success,deep+1,total_type,role);
    return success;
}
char factory(const char role,const char deep){
    for(int i=3;i<=deep;i+=2){
        if(unreal_search(i,role,1)){
            return 1;
        }
    }
    return 0;
}
