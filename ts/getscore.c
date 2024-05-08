#include"item.h"
/*
    0111110 五個1 左右兩邊不為1
    011110 活四
    211110 2011112
    
    jump four
    x10111x
    x11101x
    x11011x
    1110111
*/

char map[N][N];
Point_Score score_map[N][N];
void count_line_shape(const char *line_temp,Count_set *Counts,const char role){
    const char oppo_role=GetOppo(role);
    if(line_temp[6]!=role && line_temp[7]!=role && line_temp[4]!=role && line_temp[3]!=role) return;//篩選
    
    int middle=5,left=middle,right=middle;
    int Mid_cnt=1;
    while(line_temp[--left]==role && Mid_cnt<=5) Mid_cnt++;
    while(line_temp[++right]==role && Mid_cnt<=5) Mid_cnt++;
    if(Mid_cnt>5&&role==BLACK) {Counts->Death_count++; return;}
    if(Mid_cnt>=5){
        Counts->Five_count++;
        return;
    }
    int LBlock=(line_temp[left]==oppo_role||(role==BLACK&&line_temp[left-1]==role)),
        RBlock=(line_temp[right]==oppo_role||(role==BLACK&&line_temp[right+1]==role));
    if(Mid_cnt==4){
        //011110 x11110 01111x x1111x
        if(!LBlock&&!RBlock) Counts->Four_count++;
        else if(!(LBlock&&RBlock)) Counts->DFour_count++;
    }else{
        if(Mid_cnt==3){//特別處理
            if(line_temp[left-1]==oppo_role&&line_temp[right+1]==oppo_role&&!line_temp[left]&&!line_temp[right]) Counts->DThree_count;
            else if((line_temp[left]==oppo_role||line_temp[left-1]==oppo_role||role==BLACK&&line_temp[left-1]==role&&line_temp[left-2]==role)&&!line_temp[right]&&!line_temp[right+1]) Counts->DThree_count++;
            else if((line_temp[right]==oppo_role||line_temp[right+1]==oppo_role||role==BLACK&&line_temp[right+1]==role&&line_temp[right+2]==role)&&!line_temp[left]&&!line_temp[left-1]) Counts->DThree_count++;
        }
        int LCheck=!line_temp[left-1]&&!line_temp[left],
            RCheck=!line_temp[right+1]&&!line_temp[right];
        if(RCheck&&LCheck){
            if(Mid_cnt==3) Counts->Three_count++;
            else if(Mid_cnt==2) Counts->Two_count++;
        }else{
            //right
            if(!RCheck&&line_temp[right]!=oppo_role&&line_temp[right+1]!=oppo_role){
                int right_cnt=1,t_right=right+1;
                while(line_temp[++t_right]==role&&right_cnt<=5-Mid_cnt) right_cnt++;
                int New_RBlock=(line_temp[t_right]==oppo_role||(role==BLACK&&line_temp[t_right+1]==role));
                if(right_cnt>=5-Mid_cnt){
                    if(role==WHITE) Counts->Jfour_count++;
                }
                else if(right_cnt==4-Mid_cnt){
                    if(Mid_cnt==1) Counts->DFour_count++;
                    else Counts->Jfour_count++;
                }else if(right_cnt==3-Mid_cnt){
                    if(!New_RBlock&&!LBlock) Counts->Three_count++;
                    else if(!(New_RBlock&&LBlock)) Counts->DThree_count++;
                }
            }
            //left
            if(!LCheck&&line_temp[left]!=oppo_role&&line_temp[left-1]!=oppo_role){
                int left_cnt=1,t_left=left-1;
                while(line_temp[--t_left]==role&&left_cnt<=5-Mid_cnt) left_cnt++;
                int New_LBlock=(line_temp[t_left]==oppo_role||(role==BLACK&&line_temp[t_left-1]==role));
                if(left_cnt>=5-Mid_cnt){
                    if(role==WHITE) Counts->Jfour_count++;
                }
                else if(left_cnt==4-Mid_cnt){
                    if(Mid_cnt==1) Counts->DFour_count++;
                    else Counts->Jfour_count++;
                }else if(left_cnt==3-Mid_cnt){
                    if(!New_LBlock&&!RBlock) Counts->Three_count++;
                    else if(!(New_LBlock&&RBlock)) Counts->DThree_count++;
                }
            }
        }
    }
}


int Do_Score(const Count_set Counts,const char role){
    if(Counts.Death_count)  return 1<<31;
    else if(Counts.Five_count) return one_five;
    else if(role==BLACK&&(Counts.DFour_count+Counts.Four_count+Counts.Jfour_count>1||Counts.Three_count>=2)) return 1<<31;
    else if((Counts.DFour_count||Counts.Four_count||Counts.Jfour_count)&&Counts.Three_count) return three_four;
    else if(Counts.DFour_count+Counts.Four_count+Counts.Jfour_count>1) return three_four;
    else if(Counts.Three_count>=2) return three_three;
    else{
         return (Counts.DFour_count*death_four+Counts.DThree_count*death_three+Counts.Four_count*one_four+Counts.Three_count*one_three+Counts.Two_count*one_two+Counts.Jfour_count*jump_four)*(role==WHITE?1:2); 
    }
}
void Sum_Line(Count_set *C_S,const int y,const int x,const char role){
    *C_S=empty;
     if(role==BLACK)
        for(int i=0;i<4;i++){
            C_S->Death_count+=score_map[y][x].Black_line[i].Death_count;
            C_S->DFour_count+=score_map[y][x].Black_line[i].DFour_count;
            C_S->DThree_count+=score_map[y][x].Black_line[i].DThree_count;
            C_S->Five_count+=score_map[y][x].Black_line[i].Five_count;
            C_S->Four_count+=score_map[y][x].Black_line[i].Four_count;
            C_S->Three_count+=score_map[y][x].Black_line[i].Three_count;
            C_S->Two_count+=score_map[y][x].Black_line[i].Two_count;
            C_S->Jfour_count+=score_map[y][x].Black_line[i].Jfour_count;
        }
    else
        for(int i=0;i<4;i++){
            C_S->DFour_count+=score_map[y][x].White_line[i].DFour_count;
            C_S->DThree_count+=score_map[y][x].White_line[i].DThree_count;
            C_S->Five_count+=score_map[y][x].White_line[i].Five_count;
            C_S->Four_count+=score_map[y][x].White_line[i].Four_count;
            C_S->Three_count+=score_map[y][x].White_line[i].Three_count;
            C_S->Two_count+=score_map[y][x].White_line[i].Two_count;
            C_S->Jfour_count+=score_map[y][x].White_line[i].Jfour_count;
        }
}
void get_point_score(const int y,const int x,const char role){
    char line_temp[11]={0};
    line_temp[5]=role;
    const char oppo_role=GetOppo(role);
    
    for(int dir=0;dir<4;dir++){
        for(int flag=0;flag<2;flag++){
            for(int stretch=1;stretch<=5;stretch++){
                int next_y=y+move_y[dir]*stretch*move_dir[flag],
                    next_x=x+move_x[dir]*stretch*move_dir[flag],
                    line_pos=5+stretch*move_dir[flag];
                if(next_y>=N||next_y>>31||next_x>=N||next_x>>31||map[next_y][next_x]==oppo_role){ //邊界 or 對方
                    line_temp[line_pos]=oppo_role;
                    break;
                }
                line_temp[line_pos]=map[next_y][next_x];
            }
        }
        if(role==BLACK){
            score_map[y][x].Black_line[dir]=empty;
            count_line_shape(line_temp,&score_map[y][x].Black_line[dir],role);
        }else{
            score_map[y][x].White_line[dir]=empty;
            count_line_shape(line_temp,&score_map[y][x].White_line[dir],role);
        }
    }
    Count_set Counts={0};
    Sum_Line(&Counts,y,x,role);
    if(role==BLACK) score_map[y][x].Black_total=Do_Score(Counts,role);
    else score_map[y][x].White_total=Do_Score(Counts,role);
}

void updata_line(const int dir,const int y,const int x,const char role){
    char line_temp[11]={0};
    line_temp[5]=role;
    const char oppo_role=GetOppo(role);
    for(int flag=0;flag<2;flag++){
        for(int stretch=1;stretch<=5;stretch++){
            int next_y=y+move_y[dir]*stretch*move_dir[flag],
                next_x=x+move_x[dir]*stretch*move_dir[flag],
                line_pos=5+stretch*move_dir[flag];
            if(next_y>=N||next_y>>31||next_x>=N||next_x>>31||map[next_y][next_x]==oppo_role){ //邊界 or 對方
                line_temp[line_pos]=oppo_role;
                break;
            }
            line_temp[line_pos]=map[next_y][next_x];
        }
    }
    if(role==BLACK){
        score_map[y][x].Black_line[dir]=empty;
        count_line_shape(line_temp,&score_map[y][x].Black_line[dir],role);
    }else{
        score_map[y][x].White_line[dir]=empty;
        count_line_shape(line_temp,&score_map[y][x].White_line[dir],role);
    }
    Count_set Counts={0};
    Sum_Line(&Counts,y,x,role);
    if(role==BLACK) score_map[y][x].Black_total=Do_Score(Counts,role);
    else score_map[y][x].White_total=Do_Score(Counts,role);
}

void updata_score(const int y,const int x,const char role){
    if(map[y][x]){
        score_map[y][x]=EMPTY;
    }
    else {
        get_point_score(y,x,role);
    }
    const char oppo_role=GetOppo(role);
    for(int dir=0;dir<4;dir++){
        for(int flag=0;flag<2;flag++){
            for(int stretch=1;stretch<=5;stretch++){
                int next_y=y+move_y[dir]*stretch*move_dir[flag],
                    next_x=x+move_x[dir]*stretch*move_dir[flag];
                if(next_y>=N||next_y>>31||next_x>=N||next_x>>31||map[next_y][next_x]==oppo_role) break;
                if(!map[next_y][next_x]){
                    updata_line(dir,next_y,next_x,role);
                }
            }
        }
    }
}

int Count_role_score(const char role){
    int Score_Sum=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(!map[i][j])
                Score_Sum+=score_map[i][j].Black_total&0x7fffffff;
        }
    }
    int Score2_Sum=0;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(!map[i][j])
                Score2_Sum+=score_map[i][j].White_total&0x7fffffff;
        }
    }
    if(role==BLACK)
        return Score_Sum-Score2_Sum;
    return Score2_Sum-Score_Sum;
}




// void ppprint(int y,int x,const char role){
//     for(int i=0;i<4;i++){
//         Count_set g=role==BLACK?score_map[y][x].Black_line[i]:score_map[y][x].White_line[i];
//         printf("%d %d %d %d %d %d %d\n",g.DFour_count,g.DThree_count,g.Five_count,g.Four_count,g.Three_count,g.Two_count,g.Jfour_count);

//     }
// }
