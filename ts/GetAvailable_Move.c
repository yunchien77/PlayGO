#include"item.h"
#include "about_chess.c"

char relate_map[N][N];
int check_bigger(const int y1,const int x1,const int y2,const int x2){
    if(score_map[y1][x1].Black_total+score_map[y1][x1].White_total>score_map[y2][x2].Black_total+score_map[y2][x2].White_total){
        return 1;
    }
    return 0;
}
int *ls,*rs,*le,*re,temmp[N*N];

void sort(int* arr,const int n){
    if(n<=1) return;
    sort(arr,n/2);sort(arr+n/2,n-n/2);
    ls=arr;le=rs=arr+n/2;re=ls+n;
    for(int i=0;i<n;i++){
        if(ls==le) temmp[i]=*rs++;
        else if(rs==re) temmp[i]=*ls++;
        else if(check_bigger(toY(*rs),toX(*rs),toY(*ls),toX(*ls))) temmp[i]=*rs++;
        else temmp[i]=*ls++;
    }
    for(int i=0;i<n;i++) arr[i]=temmp[i];
}


void GetAvailable_Move(int *Can_Move,const char role){
    int flag=0,index=0;
    Count_set temp,temp2;
    int t,t2;
    const char oppo_role=GetOppo(role);
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if(relate_map[i][j]){
                Sum_Line(&temp,i,j,role);
                Sum_Line(&temp2,i,j,oppo_role);
                if(role==BLACK){
                    t=score_map[i][j].Black_total;
                    t2=score_map[i][j].White_total;
                }else{
                    t2=score_map[i][j].Black_total;
                    t=score_map[i][j].White_total;
                }
                if(t==1<<31) continue;
                else if(t==one_five){
                    if(flag<=5){
                        flag=6;
                        index=0;
                    }
                    Can_Move[index++]=toPos(i,j);
                    Can_Move[index]=-1;
                    return;
                }else if(t2==one_five&&flag<=5){
                    if(flag<=4){
                        flag=5;
                        index=0;
                    }
                    Can_Move[index++]=toPos(i,j);
                }else if((temp.Four_count||t==three_four)&&flag<=4){
                    if(flag<=3){
                        flag=4;
                        index=0;
                    }
                    Can_Move[index++]=toPos(i,j);
                }else if(temp2.Four_count&&flag<=3){
                    if(flag<=2){
                        flag=3;
                        index=0;
                    }
                    Can_Move[index++]=toPos(i,j);
                    for(int dir=0;dir<4;dir++){
                        if(i+move_y[dir]>=N||j+move_x[dir]>=N||i+move_y[dir]>>31||j+move_x[dir]>>31||i-move_y[dir]>=N||j-move_x[dir]>=N||i-move_y[dir]>>31||j-move_x[dir]>>31||!map[i+move_y[dir]][j+move_x[dir]]||!map[i-move_y[dir]][j-move_x[dir]]) continue;
                        temp=role==WHITE?score_map[i][j].Black_line[dir]:score_map[i][j].White_line[dir];
                        if(temp.Four_count){
                            for(int reve=0;reve<2;reve++){
                                for(int u=2;u<=3;u++){
                                    int next_y=i+u*move_y[dir]*move_dir[reve],
                                        next_x=j+u*move_x[dir]*move_dir[reve];
                                    if(next_y>>31||next_y>=N||next_x>>31||next_x>=N) continue;
                                    temp2=role==WHITE?score_map[next_y][next_x].Black_line[dir]:score_map[next_y][next_x].White_line[dir];
                                    if(temp2.Jfour_count&&(role==WHITE||score_map[next_y][next_x].Black_total!=1<<31)){
                                        Can_Move[index++]=toPos(next_y,next_x);
                                    }
                                }
                            }
                        }
                    }
                }else if(t2==three_four&&flag<=2){
                    if(flag<=1){
                        flag=2;
                        index=0;
                    }
                    Can_Move[index++]=toPos(i,j);
                    for(int dir=0;dir<4;dir++){
                        temp=role==WHITE?score_map[i][j].Black_line[dir]:score_map[i][j].White_line[dir];
                        if(temp.DFour_count||temp.Jfour_count||temp.Three_count){
                            for(int reve=0;reve<2;reve++){
                                for(int u=1;u<=4;u++){
                                    int next_y=i+u*move_y[dir]*move_dir[reve],
                                        next_x=j+u*move_x[dir]*move_dir[reve];
                                    if(next_y>>31||next_y>=N||next_x>>31||next_x>=N) continue;
                                    temp2=role==WHITE?score_map[next_y][next_x].Black_line[dir]:score_map[next_y][next_x].White_line[dir];
                                    if((temp2.DFour_count||temp2.Jfour_count||temp2.Three_count)&&(role==WHITE||score_map[next_y][next_x].Black_total!=1<<31)){
                                        Can_Move[index++]=toPos(next_y,next_x);
                                    }
                                }
                            }
                        }
                    }
                }
            }
    
    
    if(flag==3||flag==2){
        // if(unreal_search(Bigger_Unreal_Depth,role)){
        //     index=0;
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    if(relate_map[i][j]&&(role==WHITE||score_map[i][j].Black_total!=1<<31)){
                        Sum_Line(&temp,i,j,role);
                        if(temp.Jfour_count||temp.DFour_count)
                            Can_Move[index++]=toPos(i,j);
                    }
                }
            }
        // }

    }
    if(!flag){
        index=0;
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++)
                if(relate_map[i][j]&&(role==WHITE||score_map[i][j].Black_total!=1<<31))
                    Can_Move[index++]=toPos(i,j);
        // if(!index){
        //     for(int i=0;i<N;i++){
        //         for(int j=0;j<N;j++){
        //             printf("%d ",map[i][j]);
        //         }
        //         printf("\n");
        //     }
        // }
        sort(Can_Move,index);
        Can_Move[index]=-1;
    }else {
        if(flag>=4) index=1;
        Can_Move[index]=-1;
    }   
}


void Unreal_Get_Move(int *Can_Move,const char role,const char mode){
    /*攻擊 0
        t1自五 t2他五(阻擋時必須有四) 
        t3自任意四 
        他四三 t4四 //暫時不處理
        t5自三
        無手段 攻擊失敗
      防禦 1
        t1自五 t2他五 
        t3自活四or四三
        t4他活四
        他四三
        無手段 防禦失敗
    */
    int t1,t2,t3,t4;
    int flag=0,index=0;
    if(mode){
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(relate_map[i][j]){
                    t3=t4=0;
                    if(role==BLACK){
                        t1=score_map[i][j].Black_total;
                        if(t1==1<<31) continue;
                        t2=score_map[i][j].White_total;
                        for(int u=0;u<4;u++){
                            t3|=score_map[i][j].Black_line[u].Four_count;
                            t4|=score_map[i][j].White_line[u].Four_count;
                        }
                    }else{
                        t1=score_map[i][j].White_total;
                        t2=score_map[i][j].Black_total;
                        
                        for(int u=0;u<4;u++){
                            t3|=score_map[i][j].White_line[u].Four_count;
                            t4|=score_map[i][j].Black_line[u].Four_count;
                        }
                    }
                    if(t1==one_five){
                        index=0;
                        Can_Move[index++]=toPos(i,j);
                        Can_Move[index]=-1;
                        return;
                    }else if(t2==one_five){
                        if(flag<=2) {flag=3;index=0;}
                        Can_Move[index++]=toPos(i,j);
                    }else if((t3||t1==three_four)&&flag<=2){
                        if(flag<=1) {flag=2;index=0;}
                        Can_Move[index++]=toPos(i,j);
                    }else if(t4&&flag<=1){
                        if(!flag){flag=1;index=0;}
                        Can_Move[index++]=toPos(i,j);
                        for(int dir=0;dir<4;dir++){
                            if(i+move_y[dir]>=N||j+move_x[dir]>=N||i+move_y[dir]>>31||j+move_x[dir]>>31||i-move_y[dir]>=N||j-move_x[dir]>=N||i-move_y[dir]>>31||j-move_x[dir]>>31||!map[i+move_y[dir]][j+move_x[dir]]||!map[i-move_y[dir]][j-move_x[dir]]) continue;;
                            if((role==WHITE?score_map[i][j].Black_line[dir]:score_map[i][j].White_line[dir]).Four_count){
                                for(int reve=0;reve<2;reve++){
                                    for(int u=2;u<=3;u++){
                                        int next_y=i+u*move_y[dir]*move_dir[reve],
                                            next_x=j+u*move_x[dir]*move_dir[reve];
                                        if(next_y>>31||next_y>=N||next_x>>31||next_x>=N) continue;
                                        if((role==WHITE?score_map[next_y][next_x].Black_line[dir]:score_map[next_y][next_x].White_line[dir]).Jfour_count&&(role==WHITE||score_map[next_y][next_x].Black_total!=1<<31)){
                                            Can_Move[index++]=toPos(next_y,next_x);
                                        }
                                    }
                                }
                            }
                        }
                    }else if(t2==three_four&&!flag){
                        Can_Move[index++]=toPos(i,j);
                        for(int dir=0;dir<4;dir++){
                            if((role==WHITE?score_map[i][j].Black_line[dir]:score_map[i][j].White_line[dir]).DFour_count||(role==WHITE?score_map[i][j].Black_line[dir]:score_map[i][j].White_line[dir]).Jfour_count||(role==WHITE?score_map[i][j].Black_line[dir]:score_map[i][j].White_line[dir]).Three_count){
                                for(int reve=0;reve<2;reve++){
                                    for(int u=1;u<=4;u++){
                                        int next_y=i+u*move_y[dir]*move_dir[reve],
                                            next_x=j+u*move_x[dir]*move_dir[reve];
                                        if(next_y>>31||next_y>=N||next_x>>31||next_x>=N) continue;
                                        if(((role==WHITE?score_map[next_y][next_x].Black_line[dir]:score_map[next_y][next_x].White_line[dir]).DFour_count||(role==WHITE?score_map[next_y][next_x].Black_line[dir]:score_map[next_y][next_x].White_line[dir]).Jfour_count||(role==WHITE?score_map[next_y][next_x].Black_line[dir]:score_map[next_y][next_x].White_line[dir]).Three_count)&&(role==WHITE||score_map[next_y][next_x].Black_total!=1<<31)){
                                            Can_Move[index++]=toPos(next_y,next_x);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }else{
        int t5;
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
                if(relate_map[i][j]){
                    t3=t4=t5=0;
                    if(role==BLACK){
                        t1=score_map[i][j].Black_total;
                        if(t1==1<<31) continue;
                        t2=score_map[i][j].White_total;
                        for(int u=0;u<4;u++){
                            t3|=score_map[i][j].Black_line[u].Four_count|score_map[i][j].Black_line[u].DFour_count|score_map[i][j].Black_line[u].Jfour_count;
                            t4|=score_map[i][j].White_line[u].Four_count;
                            t5|=score_map[i][j].Black_line[u].Three_count;
                        }
                    }else{
                        t1=score_map[i][j].White_total;
                        t2=score_map[i][j].Black_total;
                        
                        for(int u=0;u<4;u++){
                            t3|=score_map[i][j].White_line[u].Four_count|score_map[i][j].White_line[u].DFour_count|score_map[i][j].White_line[u].Jfour_count;
                            t4|=score_map[i][j].Black_line[u].Four_count;
                            t5|=score_map[i][j].White_line[u].Three_count;       
                        }
                    }

                    if(t1==one_five){
                        index=0;
                        Can_Move[index++]=toPos(i,j);
                        Can_Move[index]=-1;
                        return;
                    }else if(t2==one_five){
                        if(flag<=2){
                            flag=3;
                            index=0;
                        }
                        if(t3)Can_Move[index++]=toPos(i,j);
                    }else if(t3&&flag<=2){
                        if(flag<=1){flag=2;index=0;}
                        Can_Move[index++]=toPos(i,j);
                    }else if((t4||t2==three_four)&&flag<=1){
                        if(!flag){flag=1;index=0;}
                    }else if(t5&&!flag){
                        Can_Move[index++]=toPos(i,j);
                    }
                }
            }
        }
    }
    Can_Move[index]=-1;
}