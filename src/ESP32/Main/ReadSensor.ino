/*
        front = 0 
left = 3       right = 1;
        back = 2

*/
int sensor[4] = {0 , 1 ,2 ,3 };

void setup_sensor(){
  for(int i = 0 ; i < 4 ; i ++)
  if ( i != 2)pinMode(sensor[i] , INPUT);
}

bool wallFront(){
  if( digitalRead(sensor[0]))return true ;else return false ;
}

bool wallRight(){
  if( digitalRead(sensor[1]))return true ;else return false ;
}

bool wallLeft(){
  if( digitalRead(sensor[3]))return true ;else return false ;
}