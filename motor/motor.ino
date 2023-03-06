#include <sensors.h>

// https://microcontrollerslab.com/l298n-dc-motor-driver-module-arduino-tutorial/
// If using USB, DO NOT CONNECT battery 12V to the board
// If using battery, connect EVERYTHING before switching on power. 
int ENA = 7;
int IN1 = 6;
int IN2 = 5;
int IN3 = 4;
int IN4 = 3;
int ENB = 2;

Sensors::Motor left_motor(IN4, IN3, ENB);
Sensors::Motor right_motor(IN1, IN2, ENA);

void setup() {
  left_motor.init();
  right_motor.init();
}

void loop() {
  turn_right(60);
  delay(5000);
}

void turn_right(int speed) {
  if(speed > 60){ speed = 60;}
  
  right_motor.stop();
  left_motor.forward(speed);
  delay(5000);
  right_motor.stop();
  left_motor.stop();
}

void turn_left(int speed) {
  if(speed > 60){ speed = 60;}

  right_motor.forward(speed);
  left_motor.stop();
  delay(5000);
  left_motor.stop();
  right_motor.stop();
  delay(5000);
}

void go_straight(int speed) {
  if(speed > 60){ speed = 60;}
  
  left_motor.forward(speed);
  right_motor.forward(speed);
  delay(100);
}

// void setDirectionSpeed(int motor, int dir, int speedPercentage) {
//   int pinA;
//   int pinB;
//   int pinEnable;
//   if (motor == 1){
//     pinA = IN1;
//     pinB = IN2;
//     pinEnable = ENA;
//   }
//   else{
//     pinA = IN3;
//     pinB = IN4;
//     pinEnable = ENB;
//   }
//   if (dir == 0){
//     digitalWrite(pinA, LOW);
//     digitalWrite(pinB, LOW);
//   }else if (dir == 1){
//     digitalWrite(pinA, HIGH);
//     digitalWrite(pinB, LOW);
//   }else{
//     digitalWrite(pinA, LOW);
//     digitalWrite(pinB, HIGH);
//   }

//   //max speed at 255
//   int speed = speedPercentage / 100.0 * 255;
//   analogWrite(pinEnable, speed); 

//   delay(100);
// }
