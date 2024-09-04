#include <iostream>
#include <string>
#include<utility>
#include<queue>
#include<stack>
#include<deque>
#include<vector>

#include "API.h"

#define f first 
#define s second
#define full_map for(int i = 0 ; i < 16 ; i++)for(int j = 0 ; j < 16 ; j++)

int height = 16 ;
int width = 16 ;

int visited[ 264 ] = {false};
std::vector<int>unvisited;
std::stack<int>save ;
bool node[16][16][4]  = {false}; // 0 , 1, 2, 3 // 'n' , 'e' , 's' , 'w'
int directionOffsets[4] = { 16 , 1 , -16 , -1 };
char D[4] = {'n' ,'e' ,'s' , 'w'};
int mouse_direction = 0 ;

void setup(){
    visited[0] = true ;
    for ( int i = 0 ; i < 16 ; i++){
        node[i][0][2]    = true  ;
        node[i][height - 1][0] = true;
        node[0][i][3] = true ;
        node[width - 1][i][1] = true ;
    }
    full_map API::setText(i , j , "" , i + j * height);
}

void check_wall( int x , int y){
    if ( API::wallFront() ){
        node[x][y][(mouse_direction + 0) % 4]  = true ; API::setWall(x , y , D[(mouse_direction + 0) % 4]);
    }else{
        if ( ! visited[x + y * 16 + directionOffsets[( mouse_direction + 0) % 4]]  )
        unvisited.push_back( x + y * 16 + directionOffsets[( mouse_direction + 0) % 4]); 
    }
    if ( API::wallRight() ){
        node[x][y][(mouse_direction + 1) % 4]  = true ; API::setWall(x , y , D[(mouse_direction + 1) % 4]);
    }else{
        if ( !visited[x + y * 16 + directionOffsets[( mouse_direction + 1) % 4]] )
        unvisited.push_back( x + y * 16 + directionOffsets[( mouse_direction + 1) % 4]); 
    }
    if ( API::wallLeft() ){
        node[x][y][(mouse_direction + 3) % 4]  = true ; API::setWall(x , y , D[(mouse_direction + 3) % 4]);
    }else{
        if ( !visited[x + y * 16 + directionOffsets[( mouse_direction + 3) % 4]]  )
        unvisited.push_back( x + y * 16 + directionOffsets[( mouse_direction + 3) % 4]); 
    }
    visited[ x + y * 16] = true ;
    API::setColor(x , y, 'B');
}

void move(int *ptr_x , int *ptr_y , int current_node , int next_node ,  int *ptr_mouse_direction ){
    int direction ;
    for ( int i = 0 ; i < 4 ; i++){
        if ( next_node -  current_node == directionOffsets[i])direction=i;else ;
    }
    int x = *ptr_x ; int y = *ptr_y ;
    std::pair<int , int > p[4] = {std::make_pair(0 , 1) , std::make_pair(1 , 0) , std::make_pair(0 , -1) , std::make_pair(-1 , 0)};
    switch ((direction - mouse_direction + 4) % 4)
    {
    case 1 :
        API::turnRight();
        break;
    case 3 :
        API::turnLeft();
        break;
    case 2 : 
         API::turnRight();
         API::turnRight();
    }
    *ptr_x = x + p[direction].first;
    *ptr_y = y + p[direction].second;
    *ptr_mouse_direction = direction ;
    API::moveForward();
}

bool check_nei( int x , int y){
    int check = y - x ;
    int i = x % 16 ;
    int j = x / 16 ;
    if ( check == 16 && !node[i][j][0] )return true ;
    if ( check == 1 && !node[i][j][1])return true ;
    if ( check == -16 && !node[i][j][2] )return true ;
    if ( check == -1 && !node[i][j][3] )return true ;
    return false;

}
bool check_map(){
    for ( int i = 0 ; i < 16 * 16 ; i++)
        if ( !visited[i] ) return false ;
    return true ;
}

void fillup_visited(){
    for ( int i = 0  ; i < unvisited.size() ; i++){
        int  cnt_dir = 0;
        for ( int j = 0 ; j < 4 ; j++)
        if ( unvisited[i] + directionOffsets[j] >= 0  &&  unvisited[i] + directionOffsets[j] < 16*16){
            if ( check_nei(unvisited[i] , unvisited[i] + directionOffsets[j]) && visited[unvisited[i] + directionOffsets[j]] )
            cnt_dir ++;
        }
        if (( unvisited[i] % 16 == 0 ||unvisited[i] % 16 == 15)|| ( unvisited[i] / 16 == 0 || unvisited[i] / 16 ==15))cnt_dir ++;
        if ( cnt_dir == 4) {API::setColor( unvisited[i] %16 , unvisited[i] / 16 , 'R') ;visited[unvisited[i]]=true;}
    }
}

void loop(){
    setup();
    API::setWall(0 , 0 , 's');
    int x = 0;
    int y = 0;
    int *ptr_x = &x ;
    int *ptr_y = &y ;   
    int *ptr_mouse_direction = &mouse_direction ;
    unvisited.push_back(0);
    save.push(0);
    int cnt = 0 ;
    while ( !check_map() )  {
        visited[x + y*16] = true ;
        check_wall(x , y) ;
        int next_node  = unvisited.back() ;
        fillup_visited();
        if ( check_map()){API::ackReset() ; std::cerr << "DONE";  break;}
        while ( visited[next_node]){
            unvisited.pop_back();
            next_node = unvisited.back();
        }
        if ( check_nei(save.top() , next_node)){
            move(ptr_x , ptr_y , x + y* 16 , next_node , ptr_mouse_direction);
        }else {
            while ( !check_nei (save.top() , next_node) ){
                int current = save.top();
                save.pop()  ;
                move( ptr_x  , ptr_y , current , save.top() , ptr_mouse_direction);
            }
            move( ptr_x ,ptr_y , save.top() , unvisited.back() , ptr_mouse_direction);
        }
        save.push(next_node);
        unvisited.pop_back();
        }
    }

