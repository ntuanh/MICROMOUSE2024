/*
thông số chân:
 in1 25, in2 26, pwma 27, en1a 5, en1b 17, (trái)      
 in3 32, in4 33, pwmb 14, en2a19, en2b 18 (phải)




R xe = 1,5cm
xung 210 
tỉ số truyền 1:30




*/

// Encoder pins
#define ENCA1 5  // Encoder A (Left motor)
#define ENCB1 17 // Encoder B (Left motor)
#define ENCA2 19 // Encoder A (Right motor)
#define ENCB2 18 // Encoder B (Right motor)

// Motor A (Left)
#define PWM_A 27
#define IN1_A 25
#define IN2_A 26

// Motor B (Right)
#define PWM_B 14
#define IN3_B 32
#define IN4_B 33

volatile long posLeft = 0;
volatile long posRight = 0;

long prevT = 0;
float eprev = 0;
float eintegral = 0;

#define INPUT_DISTANCE 150  // in mm (converted from cm to match TOF10120 units)
#define ERROR_DIST 50  // in mm (converted to mm)
#define SPEED 95
#define MAX_DIFFERNCE 10
#define MAX_TURN_SPEED 10 
#define CALIBRATION 3
#define MAX_ALLIGN_ANGLE 5  // in mm
#define COLLISION_DISTANCE 100  // in mm

// Constants for calibration
const float Kp = 0.1;   // Proportional constant to fix straight movement
const float Kd = 0.02;  // Derivative constant for adjusting drift
const float Ki = 0.02;   // Integral constant

const int targetTicksStraight = 210;  // Adjusted ticks for forward movement
const int targetTicks = 210;  //  Adjusted encoder ticks for 90-degree turn 
const int speed = 255; //1-255

void setupMotor()
{
  pinMode(ENCA1, INPUT);
  pinMode(ENCB1, INPUT);
  pinMode(ENCA2, INPUT);
  pinMode(ENCB2, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCA1), readEncoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCA2), readEncoderRight, RISING);
  
  // Initialize motor pins
  pinMode(PWM_A, OUTPUT);
  pinMode(IN1_A, OUTPUT);
  pinMode(IN2_A, OUTPUT);
  
  pinMode(PWM_B, OUTPUT);
  pinMode(IN3_B, OUTPUT);
  pinMode(IN4_B, OUTPUT);
}

// Function to read encoder value for the left motor
void readEncoderLeft() {
  if (digitalRead(ENCB1) > 0) {
    posLeft++;
  } else {
    posLeft--;
  }
}

// Function to read encoder value for the right motor
void readEncoderRight() {
  if (digitalRead(ENCB2) > 0) {
    posRight++;
  } else {
    posRight--;
  }
}

// Function to control motor direction and speed
void setMotor(float control, int pwmPin, int in1, int in2) {
  int pwr = abs(control);
  if (pwr > 255) pwr = 255;

  analogWrite(pwmPin, pwr);
  
  if (control > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (control < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

// Function to stop both motors
void stopMotors() {
  setMotor(0, PWM_A, IN1_A, IN2_A);
  setMotor(0, PWM_B, IN3_B, IN4_B);
}

//Function to move forward while keeping the robot straight
void moveForward() {
  posLeft = 0;
  posRight = 0;
  int pos = 0;
  // while (abs(pos) < targetTicksStraight) {
  //   //Left motor forward, right motor backward for turning
  //   noInterrupts();
  //   pos = (posLeft + posRight) / 2;
  //   interrupts();
  //   int error = targetTicksStraight - abs(pos);
  //   float PID = 1 * error;
  //   setMotor(PID, PWM_A, IN1_A, IN2_A);   // Adjust speed as necessary
  //   setMotor(-PID, PWM_B, IN3_B, IN4_B);  // Adjust speed as necessary
  //   Serial.println(posRight);
  //   delay(1);
  // }
  while (abs(posRight) < targetTicksStraight && abs(posLeft) < targetTicksStraight) {
  //Left motor forward, right motor backward for turning
    setMotor(speed, PWM_A, IN1_A, IN2_A);   // Adjust speed as necessary
    setMotor(-speed, PWM_B, IN3_B, IN4_B);  // Adjust speed as necessary
    delay(1);
  }
  stopMotors();  // Stop after turning 90 degrees
}
// Function to perform a 90-degree left turn
void turnLeft(){
  posLeft = 0;
  posRight = 0;

  while (abs(posLeft) < targetTicks && abs(posRight) < targetTicks) {
    
    // Calculate the difference in encoder counts between left and right motors
    int error = posRight - posLeft;
    
    // Adjust motor speeds to minimize the drift
    float correction = Kp * error + Kd * (eprev - error);// + Ki * eintegral;
    eintegral += error;
    eprev = error;
    
    setMotor(speed - correction, PWM_A, IN2_A, IN1_A);  // Adjust left motor speed
    setMotor(speed + correction, PWM_B, IN4_B, IN3_B);  // Adjust right motor speed
    delay(5);  // Tốc độ vòng lặp
  }

  stopMotors();  // Stop after moving forward the set distance
}
// Function to perform a 90-degree right turn
void turnRight() {
  posLeft = 0;
  posRight = 0;

  while (abs(posLeft) < targetTicks && abs(posRight) < targetTicks) {
    // Calculate the difference in encoder counts between left and right motors
    int error = posLeft - posRight;
    
    // Adjust motor speeds to minimize the drift
    float correction = Kp * error + Kd * (eprev - error);// + Ki * eintegral;
    eintegral += error;
    eprev = error;
    setMotor(speed - correction, PWM_A, IN1_A, IN2_A);  // Adjust left motor speed
    setMotor(speed + correction, PWM_B, IN3_B, IN4_B);  // Adjust right motor speed
    delay(5);  // Tốc độ vòng lặp
  }
  stopMotors();  // Stop after moving forward the set distance
}
void turnaround(){
  int targetTicksTurn = targetTicks*2;
  posLeft = 0;
  posRight = 0;

  while (abs(posLeft) < targetTicksTurn && abs(posRight) < targetTicksTurn) {
    // Calculate the difference in encoder counts between left and right motors
    int error = posLeft - posRight;
    
    // Adjust motor speeds to minimize the drift
    float correction = Kp * error + Kd * (eprev - error) + Ki * eintegral;
    eintegral += error;
    eprev = error;
    setMotor(speed - correction, PWM_A, IN1_A, IN2_A);  // Adjust left motor speed
    setMotor(speed + correction, PWM_B, IN3_B, IN4_B);  // Adjust right motor speed
    delay(5);  // Tốc độ vòng lặp
  }
}


