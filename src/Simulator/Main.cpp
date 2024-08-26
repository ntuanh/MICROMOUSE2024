#include <iostream>
#include <string>
#include<utility>
#include<queue>


#include "API.h"

#define f first 
#define s second
#define fullmap for(int i = 0 ; i < 16 ; i++)for(int j = 0 ; j < 16 ; j++)

int width = API::mazeWidth();
int height = API::mazeHeight();
int N_MAX = 100;

std::pair<int, int> center[5];
std::pair<int ,int > add [4];
char D[4] = {'n' ,'e' ,'s' , 'w'};
int steps[18][18];
int stepsback[18][18];
bool node[18][18][4] = {false};

int mouse_direction ;

void log(const std::string& text , int number ) {
    std::cerr << text << number << " ";
}

void setup(){
    mouse_direction = 0 ;
    center[1] = {8 , 8};
    center[2] = {7 , 8};
    center[3] = {7 , 7};
    center[4] = {8 , 7};   
    for ( int i = 0 ; i < 16 ; i++){
        node[i][0][2]    = true  ;
        node[i][height - 1][0] = true;
        node[0][i][3] = true ;
        node[width - 1][i][1] = true ;
    }
}

void refill(){
    fullmap steps[i][j]= N_MAX ;

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

void display(){
    fullmap{
            API::setText(i ,j , "" , steps[i][j]);
            if(node[i][j][0] )API::setWall(i , j ,'n');
            if(node[i][j][1] )API::setWall(i , j ,'e');
            if(node[i][j][2] )API::setWall(i , j, 's');
            if(node[i][j][3] )API::setWall(i , j ,'w');
        }
}

void Update_node(){
    fullmap{
            if(node[i][j][0] == true && j + 1 < height)node[i][j+1][2] = true;
            if(node[i][j][1] == true && i + 1 < width )node[i+1][j][3] = true;
            if(node[i][j][2] == true && j - 1 >= 0)node[i][j-1][0] = true ;
            if(node[i][j][3] == true && i - 1 >= 0)node[i-1][j][1] = true;
        }
}

bool check_node_center(int x , int y){
    for ( int i = 1 ; i <= 4 ; i++)
    if ( x == center[i].f && y == center[i].s)return true ;
    return false;
}

void fillback(){
    std::queue<std::pair<int ,int> > Q;
    Q.push({0,0});
    for ( int i = 0 ; i < width ; i++){
        for ( int j = 0 ; j <height ; j++){
            stepsback[i][j] = N_MAX;
        }
    }

    stepsback[0][0] = 0;

    while ( Q.empty() == false ){
        int x = Q.front().f;
        int y = Q.front().s;
        Q.pop();
        for ( int i = 0 ; i < 4 ; i++){
            if (node[x][y][i] == false && stepsback[x + add[i].f][ y + add[i].s] > stepsback[x][y] + 1){
                stepsback[x + add[i].f][y + add[i].s] = stepsback[x][y] + 1;
                Q.push(std::make_pair(x + add[i].f , y + add[i].s));
            }
        }
    }

    for ( int i = 0 ; i < width ; i++){
        for ( int j = 0; j < height ; j++){
            API::setText(i , j , "",stepsback[i][j]);
        }
    }
}

void check_wall( int x , int y){
    if ( API::wallFront() == true ){
        node[x][y][(mouse_direction + 0) % 4]  = true ; API::setWall(x , y , D[(mouse_direction + 0) % 4]);
    }
    if ( API::wallRight() == true ){
        node[x][y][(mouse_direction + 1) % 4]  = true ; API::setWall(x , y , D[(mouse_direction + 1) % 4]);
    }
    if ( API::wallLeft() == true ){
        node[x][y][(mouse_direction + 3) % 4]  = true ; API::setWall(x , y , D[(mouse_direction + 3) % 4]);
    }   
    API::setColor(x , y, 'B');
}

int main(int argc, char* argv[]) {
    setup();
    refill();
    display();
    mouse_direction = 0 ;
    add[0] = {0 , 1};
    add[1] = {1 , 0};
    add[2] = {0 , -1};
    add[3] = {-1 , 0}; 
    int cnt = 0;
    int x = 0 ;
    int y = 0;
    bool check = false ;
    bool check_center = false;
    bool check_back = false;
    int loop = 0;
    while  ( loop < 7 ){
        loop ++;
    // go to center
        refill();
        check_center = false ;
        check_back = false;
        while ( !check_center ){
            Update_node();
            refill();
            display();
            check_wall(x, y);
            for ( int i = 0 ; i < 4 ; i++){
                if ( node[x][y][i] == false && steps[x][y] > steps[x + add[i].f][y + add[i].s]){
                        int res = (i - mouse_direction + 4) % 4 ;
                        mouse_direction = i ;
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
                    x += add[i].f;
                    y += add[i].s;
                    break;
                }
            if ( steps[x][y] == 0)check_center = true;
            }
        }
        fillback();
    // back to start 
        while ( check_back == false ){
            Update_node();
            fillback();
            check_wall(x,y);
            std::cerr << cnt << '\n';
            cnt = N_MAX * 10;
            for ( int i = 0 ; i < 4 ; i++){
                if ( node[x][y][i] == false && stepsback[x][y] > stepsback[x + add[i].f][y + add[i].s]){
                        int res = (i - mouse_direction + 4) % 4 ;
                        mouse_direction = i ;
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
                    x += add[i].f;
                    y += add[i].s;
                    break;
                }
                if ( cnt > stepsback[center[i + 1].f ][center[i+1].s]) cnt = stepsback[center[i + 1].f ][center[i+1].s];
            }
            if (stepsback[x][y] == 0)check_back = true;
        }
        if ( cnt > steps[0][0])loop++;else loop +=10 ;
    }
       
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
                x += add[i].f;
                y += add[i].s;
                break;
            }
        }
    }
}