#pragma once

#include <string>

/* define pin */ 

//Driver 
const int PWM_A = 11 ; // standby 
const int AIN_1 =  13 ; // forward 
const int AIN_2 =  12 ;
const int PWM_B = 8 ;
const int BIN_1 =  10 ; // forward 
const int BIN_2 = 9 ;
const int speed =  255 ;

// encoder right 
const int EnA_R = 22 ;
  const int EnB_R = 23 ;

// encoder left 
const int EnA_L = 24 ;
const int EnB_L = 25 ;


class API {

public:

    static int mazeWidth();
    static int mazeHeight();

    static bool wallFront();
    static bool wallRight();
    static bool wallLeft();

    static void moveForward(int distance = 1);
    static void turnRight();
    static void turnLeft();

    static void setWall(int x, int y, char direction);
    static void clearWall(int x, int y, char direction);

    static void setColor(int x, int y, char color);
    static void clearColor(int x, int y);
    static void clearAllColor();

    static void setText(int x, int y, const std::string& text , int number);
    static void clearText(int x, int y);
    static void clearAllText();

    static bool wasReset();
    static void ackReset();

};
