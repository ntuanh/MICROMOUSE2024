// const define
/*
(1) Motor A : minimum speed 100
PWA : 10 ;
AIN2 : 9 ;
AIN1 : 8 ;
EN1_A : 3 ;
EN2_A : 2  ;

(2) Mortor B : minimum speed 150 
PWB : 10 ;
BIN2 : 9 ;
BIN1 : 8 ;
EN1_B : 3 ;
EN2_B : 2  ;

(3) general : 

stb : 7 ;



*/

// Programming 
//(1) Library :
#include<Arduino_FreeRTOS.h>
#include <semphr.h>



// define :

int PWA = 10 ;
int AIN2 = 9 ;
int AIN1 = 8 ;
int EN1_A = 2 ;
int EN2_A = 3  ;
volatile int  val_A = 0 ;

int PWB = 4 ;
int BIN2 = 6 ;
int BIN1 = 5 ;
int EN1_B = 11 ;
int EN2_B = 12  ;
volatile int val_B = 0 ;

int standby = 7 ;


void Task_1( void *pvParameters);
void Task_2( void *pvParameters);
void Task_3( void *pvParameters);

void setup(){
  
  pinMode(standby , OUTPUT);
//(1) motor A: 
  pinMode(PWA , OUTPUT);
  pinMode(AIN1  , OUTPUT);
  pinMode(AIN2 ,  OUTPUT);
  pinMode(EN1_A , INPUT);
  pinMode(EN2_A , INPUT);
//(2) motor B :
  pinMode(PWB , OUTPUT);
  pinMode(BIN1  , OUTPUT);
  pinMode(BIN2 ,  OUTPUT);
  pinMode(EN1_B , INPUT_PULLUP);
  pinMode(EN2_B , INPUT_PULLUP);
  
//(4) Read encoder : 
  attachInterrupt(digitalPinToInterrupt(EN1_A), readEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(EN1_B), readEncoderB, RISING);
  Serial.begin(9600);
  
  
  xTaskCreate(Task_1 , "ReadMotorA" , 128 , NULL , 1 , NULL );
  xTaskCreate(Task_2 , "ReadMotorB" , 128 , NULL , 1 , NULL );
  xTaskCreate(Task_3 , "DISPLAY" , 128 , NULL , 1 , NULL );

  vTaskStartScheduler();
}

void move( bool motorA , int speed , bool direction){
    digitalWrite(standby , HIGH);
    int pmw = PWA , in1  = AIN1, in2 = AIN2 ;
    if ( !motorA ){
        pmw = PWB ; in1 = BIN1 ; in2 = BIN2 ;
    }

    analogWrite( pmw , speed);
    digitalWrite(in1 , direction);
    digitalWrite(in2 , 1 - direction);
}

void readEncoderA(){ 
  if(digitalRead(EN1_A) > digitalRead(EN2_A) ) val_A --; else val_A ++;
  //Serial.println(" interupted A ");
}

void readEncoderB(){ 
  if(digitalRead(EN1_B) > digitalRead(EN2_B) ) val_B --; else val_B ++;
  //Serial.println(" interupted B ");
}

void stand(){digitalWrite(standby , LOW);}


void Task_1( void *pvParameters){
  (void)pvParameters ;
  while(true ){
    move(1 , 100 , 1);
    readEncoderA();
  }
}

void Task_2( void *pvParameters){
  (void)pvParameters ;
  while(true ){
    move(0 , 150 , 1);
    readEncoderB();
  }
}

void Task_3( void *pvParameters){
  (void)pvParameters ;
  while(true){
    Serial.print("Value A is : ");
    Serial.print(val_A);
    Serial.print(" || Value B is : ");
    Serial.println(val_B);
    //vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void loop(){}