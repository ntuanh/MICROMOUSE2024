#include <iostream>
#include <string>
#include<utility>
#include<queue>

#include "API.h"

std::pair<int , int > center[5] ;


void setup() 
{
    Serial.begin(115200);
    center[1] = {7 , 7};
    center[2] = {8 , 8};
    center[3] = {7 , 8};
    center[4] = {8 , 7};
}

bool check( int x , int y ) 
{
    for ( int i = 1 ; i < 5 ; i++)
    if ( x == center[i].first && y == center[i].second)
    return true ;
    return false ;
}

void move ( int *ptr_x  , int *ptr_y , int *ptr_dir)
{
    if (!API::wallLeft() ){
        (*ptr_dir) += 3;
        API::turnLeft() ;
        API::moveForward();
    }else {
        if(!API::wallFront())API::moveForward() ;
        else {
            if (!API::wallRight() ){
            API::turnRight() ;
            (*ptr_dir)++;
            }else {
                API::turnLeft();
                API::turnLeft();
                (*ptr_dir) += 2;
            }
            API::moveForward();
        }
    }

    (*ptr_dir) = (*ptr_dir)  % 4;
    std::pair <int , int > add[4] = {{0, 1}, {1 , 0} , {0 , -1} , {-1 , 0}};

    (*ptr_x) += add[(*ptr_dir)].first;
    (*ptr_y) += add[(*ptr_dir)].second;
}

void loop(){
    //setup();
    int mouse_direction = 0 ;
    int x = 0 ;
    int y = 0 ;
    int *ptr_x = &x ;
    int *ptr_y = &y ;
    int *pir_dic = &mouse_direction ;
    int cnt = 0 ;
    while ( cnt < 10 || !check( x, y) ){
        move(ptr_x , ptr_y  , pir_dic);
        std::cerr << *ptr_x << " " << *ptr_y << '\n';
        cnt ++ ;
    }
}
