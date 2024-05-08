#ifndef ITEM
    #define ITEM
    #define N 23
    #define WHITE 2
    #define BLACK 1
    #define GetOppo(Role) (Role==WHITE?BLACK:WHITE)
    #define toX(Pos) (Pos&0b1111111111)
    #define toY(Pos) (Pos>>10)
    #define toPos(Y,X) (((Y)<<10)+(X))
    #define Depth 3
    #define Root_Unreal_Depth 5
    #define Bigger_Unreal_Depth 15
    #define range 2
    #define Table_Memory 25//16Byte
    #define EXACT 1

    #define one_two 12
    #define one_three 45
    #define one_four 100
    #define one_five 5000
    #define three_four 4999
    #define three_three 4998
    #define win_score 100000
    #define death_three 10
    #define death_four 40
    #define jump_four 40
    extern char map[N][N],relate_map[N][N];
    extern int step,step2;
    const int move_x[]={-1,0,1,1},move_y[]={-1,-1,-1,0},move_dir[]={1,-1};
    typedef struct{
    int Five_count,
        DFour_count,
        Four_count,
        DThree_count,
        Three_count,
        Two_count,
        Death_count,
        Jfour_count;
    }Count_set;
    
    typedef struct{
        Count_set Black_line[4],White_line[4];
        int Black_total,White_total;
    }Point_Score;
    const Count_set empty;
    const Point_Score EMPTY;
    extern Point_Score score_map[N][N];

#endif

