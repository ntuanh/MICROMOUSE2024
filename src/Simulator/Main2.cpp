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
const int n = 16 ;

int visited[ 264 ] = {false};
std::vector<int>unvisited;
std::stack<int>save ;
std::pair<int ,int > add [4];
int steps[n][n];
bool node[n][n][4]  = {false}; // 0 , 1, 2, 3 // 'n' , 'e' , 's' , 'w'
int directionOffsets[4] = { n, 1 , -n , -1 };
char D[4] = {'n' ,'e' ,'s' , 'w'};
int mouse_direction = 0 ;

std::pair<int, int> center[5];

void setup(){
    visited[0] = true ;
    for ( int i = 0 ; i < n; i++){
        node[i][0][2]    = true  ;
        node[i][height - 1][0] = true;
        node[0][i][3] = true ;
        node[width - 1][i][1] = true ;
    }
    full_map API::setText(i , j , "" , i + j * height);
    add[0] = {0 , 1};
    add[1] = {1 , 0};
    add[2] = {0 , -1};
    add[3] = {-1 , 0}; 
    center[1] = {15 , 15};
    center[2] = {15 , 15};
    center[3] = {15 , 15};
    center[4] = {15 , 15};  
    
    node[1][1][0]= true ;

}

void Update_node(){
    full_map{
        for (int k = 0 ; k < 4 ; k++ )
        if( node[i][j][k] && ( i + add[k].f < n && i + add[k].f >= 0) && ( j + add[k].s >=0  && j + add[k].s < n))
        node[i + add[k].f][j + add[k].s][(k + 2) % 4] = true ;
    }
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
    int cnt = unvisited.size();
    for ( int i = 0 ; i < unvisited.size() ; i++)
    if( visited[unvisited[i]])cnt --;
    if ( cnt == 0)return true ; else return false;

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
        if ( !visited[unvisited[i]] && cnt_dir == 4 ) {
            //std::cerr << " check " << i << std::endl;
            int temp_x = unvisited[i] % 16 ;
            int temp_y = unvisited[i] / 16 ;    
            API::setColor( temp_x , temp_y, 'R') ;visited[unvisited[i]]=true;
            for ( int l = 0 ; l < 4 ; l++)node[temp_x][temp_y][l]=true;           
        }
    }
}

void find_center(){
    for ( int i = 0 ; i + 1 < 16 ; i++)
    for ( int j = 0 ; j + 1 < 16 ; j++)
    if ( !node[i][j][0] && !node[i][j][1] && !node[i+1][j+1][2] && !node[i+1][j+1][3])
    {
        std::cerr <<" center " << i << " " << j << '\n';
        center[1] = {i + 1 , j + 1};
        center[2] = {i , j + 1 };
        center[3] = {i , j};
        center[4] = {i + 1  , j};
    }

}

void refill(){
    full_map steps[i][j]= 100000;

    for ( int i = 1 ; i < 5 ; i++){
        API::setText(center[i].f , center[i].s , "" , 0);
        steps[center[i].f][center[i].s] =  0;
    }

    std::queue< std::pair< int , int>> Q ;
    for ( int i = 1 ; i < 5 ; i++){ Q.push(center[i]);}
    while ( Q.empty() == false ){
        std::pair<int ,int >temp ;
        temp = Q.front();
        Q.pop();
        int x = temp.f ;
        int y = temp.s;
        if ( node[x][y][0] == false && steps[x][y+1] > steps[x][y] + 1){
            steps[x][y+1] = steps[x][y] + 1;
            Q.push(std::make_pair(x , y + 1));
        }

        if ( node[x][y][1] == false && steps[x + 1][y] > steps[x][y] + 1){
            steps[x + 1][y] = steps[x][y] + 1;
            Q.push(std::make_pair(x + 1, y ));
        }

        if ( node[x][y][3] == false && steps[x - 1][y] > steps[x][y] + 1){
            steps[x - 1][y] = steps[x][y] + 1;
            Q.push(std::make_pair(x -1 , y ));
        }

        if ( node[x][y][2] == false && steps[x][y -1] > steps[x][y] + 1){
            steps[x ][y -1] = steps[x][y] + 1;
            Q.push(std::make_pair(x , y - 1));
        }
    }
}


void move_1( int res ){
    switch (res){
        case 3:
            API::turnLeft();
                break;
        case 1 :
            API::turnRight();
                break;
        case 2 :
            API::turnRight();
            API::turnRight();
                break;
    }
    API::moveForward();
}

void display(){
    full_map{
            API::setText(i ,j , "" , steps[i][j]);
            for (int k = 0 ; k < 4 ; k++)
            if (node[i][j][k])API::setWall(i , j , D[k]);
        }
}

void show_visited(){
    std::cerr << " show visited \n";
    for( int i = 0 ; i < n * n ; i++)if (visited[i] )std::cerr << i << " ";
    std::cerr << std::endl;
}

void show_unvisited(){
    std::cerr << " show unvisited \n";
    for(int i = 0 ; i < unvisited.size() ; i++)std::cerr << unvisited[i] << " ";
    std::cerr << std::endl;
}
int main( int argc , char* argv[]){
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
    while (true  )  {
        visited[x + y*16] = true ;
        //show_visited();
        show_unvisited();
        
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
        //Update_node();
        //find_center();
        }
    refill();
    find_center();
    
        bool check_center = false ;
        bool check_back = false;
        mouse_direction = 0 ;
        x = y = 0 ;
        while ( steps[x][y] != 0){
        Update_node();
        refill();
        display();
        check_wall(x, y);
        API::setColor(x, y, 'G');
        for ( int i = 0 ; i < 4 ; i++){
            if ( node[x][y][i] == false && steps[x][y] > steps[x + add[i].f][y + add[i].s]){
                int res = (i - mouse_direction + 4) % 4 ;
                mouse_direction = i ;
                move_1(res);
                x += add[i].f;
                y += add[i].s;
                break;
            }
        }
    }
        

    
}

