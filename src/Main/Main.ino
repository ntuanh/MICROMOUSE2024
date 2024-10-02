#include <iostream>
#include <string>
#include<utility>
#include<queue>

#include "API.h"

std::pair<int , int > center[5] ;

TaskHandle_t Task1;   // reading laser sensor 
TaskHandle_t Task2;   // controller driver 

void setup() 
{
  Serial.begin(115200);
  // dual core setup 
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Reading Sensor ",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "ControllerDriver",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 

    setup_reading_sensor();
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

void Mouse()
{
    //setup();
    int mouse_direction = 0 ;
    int x = 0 ;
    int y = 0 ;
    int *ptr_x = &x ;
    int *ptr_y = &y ;
    int *pir_dic = &mouse_direction ;
    int cnt = 0 ;
    while ( !check( x, y) ){
        move(ptr_x , ptr_y  , pir_dic);
        std::cerr << *ptr_x << " " << *ptr_y << '\n';
        cnt ++ ;
    }
}

//Task1code: read laser sensor and update to API.h
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
    laser_processing();
  } 
}

//Task2code: ... 
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
    Mouse();
  }
}


void loop()
{
}



