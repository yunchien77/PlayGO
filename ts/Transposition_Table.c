#include "item.h"
#define _CRT_RAND_S
#include<stdlib.h>
#include<time.h>
const int mask=(1<<Table_Memory)-1;
int Trans_index;
typedef struct
{
    long long int check_num;
    int value;
    signed char deep;
    //128         64            32     16      8         4             2      1
    //real_type   have_unreal_w type_w value_w have_real have_unreal_b type_b value_b
    unsigned char a_lot_of_item;
    signed char deep_black_v,deep_white_v;
}BIG_MAP;
long long int W_table[N][N],B_table[N][N],The_Key;
BIG_MAP *big_map;

long long int ran64(){
    // unsigned int ran_temp1,ran_temp2;
    // if(rand_s(&ran_temp1)||rand_s(&ran_temp2)){
    //     printf("rand set error");
        return (long long int)rand()<<60|((long long int)rand()<<45)|((long long int)rand()<<30)|(rand()<<15)|rand(); 
    // }
    
    // return (long long int)(ran_temp1)<<32|ran_temp2;
}
void initial_table(){
    srand(time(NULL));
    for(int j=0;j<N;j++)
        for(int i=0;i<N;i++){
            W_table[i][j]=ran64();
            B_table[i][j]=ran64();
        }
    The_Key=ran64();
    big_map=(BIG_MAP*)calloc(1<<Table_Memory,sizeof(BIG_MAP));
}
void free_big_map(){free(big_map);}
void updata_key(const int y,const int x,const int role){
    The_Key^=role==WHITE?W_table[y][x]:B_table[y][x];
}

void updata_value(const int value,const signed char deep,const char type){
    Trans_index=The_Key&mask;
    big_map[Trans_index].check_num=The_Key;
    big_map[Trans_index].deep=deep;
    big_map[Trans_index].value=-value;
    big_map[Trans_index].a_lot_of_item&=0b01111111;
    big_map[Trans_index].a_lot_of_item|=type<<7;
    big_map[Trans_index].a_lot_of_item|=8;
}
// int usf;
int try_get_value(int *value,signed char *deep,char *type){
    Trans_index=The_Key&mask; 
    if(big_map[Trans_index].check_num==The_Key&&big_map[Trans_index].a_lot_of_item&8){ 
        // printf("%d\n",usf++);
        *value=big_map[Trans_index].value;
        *type=big_map[Trans_index].a_lot_of_item>>7;
        *deep=big_map[Trans_index].deep;
        return 1;
    }
    return 0;
}
int try_get_value_v(char *value,const signed char deep,const char role){
    Trans_index=The_Key&mask;
    
    if(big_map[Trans_index].check_num==The_Key){
        if(role==BLACK){
            if(big_map[Trans_index].a_lot_of_item&4 && (big_map[Trans_index].a_lot_of_item&2||big_map[Trans_index].deep_black_v>=deep)){
                *value=big_map[Trans_index].a_lot_of_item&1;
                return 1;
            }
        }else{
            if(big_map[Trans_index].a_lot_of_item&64 && (big_map[Trans_index].a_lot_of_item&32||big_map[Trans_index].deep_white_v>=deep)){
                *value=big_map[Trans_index].a_lot_of_item&16;
                return 1;
            }
        }
    }
    return 0;
}
void updata_value_v(const char value,const signed char deep,const char type,const char role){
    Trans_index=The_Key&mask;
    big_map[Trans_index].check_num=The_Key;
    if(role==BLACK){
        big_map[Trans_index].deep_black_v=deep;
        big_map[Trans_index].a_lot_of_item&=0b11111100;
        big_map[Trans_index].a_lot_of_item|=value;
        big_map[Trans_index].a_lot_of_item|=type<<1;
        big_map[Trans_index].a_lot_of_item|=4;
    }else{
        big_map[Trans_index].deep_white_v=deep;
        big_map[Trans_index].a_lot_of_item&=0b11001111;
        big_map[Trans_index].a_lot_of_item|=value<<4;
        big_map[Trans_index].a_lot_of_item|=type<<5;
        big_map[Trans_index].a_lot_of_item|=0b01000000;
    }
}
char get_type(){
    Trans_index=The_Key&mask;
    if(big_map[Trans_index].check_num==The_Key){
        return big_map[Trans_index].a_lot_of_item>>7;
    }
    return !EXACT;
}
char get_type_v(){
    Trans_index=The_Key&mask;
    if(big_map[Trans_index].check_num==The_Key){
        return big_map[Trans_index].a_lot_of_item&2;
    }
    return !EXACT;
}
