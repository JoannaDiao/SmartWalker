#include <Servo.h>
#include <sensors.h>
#include "pitches.h"

typedef enum {
  INIT,
  NO_INTERFERENCE,
  ASSIST_LEFT_TURN,
  ASSIST_RIGHT_TURN,
  LONG_BRAKE,
  SHORT_BRAKE
} robot_state_t;

robot_state_t robot_state = INIT;
int left_motor_power = 0;
int right_motor_power = 0;
double FL_floor_avg;
double FR_floor_avg;
int left_led = 49;
int right_led = 47;
int pos = 0;

Sensors::TOF BR_TOF(4); // rear right TOF
Sensors::TOF BL_TOF(5); // rear left TOF
Sensors::TOF FL_TOF(6); // front left TOF
Sensors::TOF FR_TOF(7); // front right TOF

int ENA = 7;
int IN1 = 6;
int IN2 = 5;
int IN3 = 4;
int IN4 = 3;
int ENB = 2;
Sensors::Motor left_motor(IN4, IN3, ENB);
Sensors::Motor right_motor(IN1, IN2, ENA);

Servo servo;

Sensors::Grip left_grip(44);
Sensors::Grip right_grip(48);

const int TURN_MOTOR_VALUE = 60;
const int STOP_MOTOR_VALUE = 0;
const int LONG_BRAKE_TIME = 10000;
const int SHORT_BRAKE_TIME = 1500;

void setState(robot_state_t new_state) {
  robot_state = new_state;
}

void floorCalibration(){
  double left_floor = 0;
  double right_floor = 0;
  for (int i = 0; i < 124; i++){
    double left_dist = FL_TOF.getDistance();
    double right_dist = FR_TOF.getDistance();

    //get front sensors to right values as well by just calling the TOFs
    BR_TOF.getDistance();
    BL_TOF.getDistance();
    
    if( i == 123){
      right_floor = right_dist;
      left_floor = left_dist;
    }
   }

   FL_floor_avg = left_floor;
   FR_floor_avg = right_floor;
}

bool sittingDetected() {
  delay(50);
  double bl_dist = BL_TOF.getDistance();
  double br_dist = BR_TOF.getDistance();

  Serial.print("    BR: ");
  Serial.print(br_dist);
  Serial.print(", BL: ");
  Serial.print(bl_dist);
  Serial.println();
  
  if (bl_dist == -1 || br_dist == -1) {
    return false;
  } 
  
  if (bl_dist < 25 && br_dist < 25) {
    return true;
  } 
  
  return false;
}

bool handlesEngaged() {
  bool lg = left_grip.handleEngaged();
  bool rg = right_grip.handleEngaged();

  if( rg && lg){
      return true;
  }
  return false;
}

bool userWantsToSit() {
  // if user is touching the grip && back facing TOF distance decrease below a certain threshold --> user wants to sit down
  bool g = handlesEngaged();
  bool s = sittingDetected();
  if (g && s) {
    return true;
  }
  return false;
}

void Brake() {
  delay(100);
  for (pos = 80; pos <= 170; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  delay(750);
}

void Unbrake() {
  delay(100);
  for (pos = 170; pos >= 80; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);
    delay(10); // waits 15ms for the servo to reach the position
  }
  delay(750);
}

void handleLongBrake() {
  Serial.println("handleLongBrake");
  Brake();
  
  while(sittingDetected()){
    delay(100);
  }
  Unbrake();
  setState(NO_INTERFERENCE);
}

void handleShortBrake() {
  Serial.println("handleShortBrake");
  tone(45, NOTE_C7, 1000 / 8);
  digitalWrite(right_led, HIGH);
  digitalWrite(left_led, HIGH);
  delay(100);
  Brake();
  noTone(45);
  tone(45, NOTE_C7, 1000 / 8);
  delay(SHORT_BRAKE_TIME);
  digitalWrite(right_led, LOW);
  digitalWrite(left_led, LOW);
  Unbrake();
  noTone(45);

  delay(3000);
  for(int i=0; i<5; i++) {
    double left_dist = FL_TOF.getDistance();
    double right_dist = FR_TOF.getDistance();
  }
  
  setState(NO_INTERFERENCE);
}

void handleInit() {
  Serial.println("handleInit");
  floorCalibration();
  delay(500);

  for (int i = 0; i < 2; i++) {
    digitalWrite(left_led, HIGH);
    digitalWrite(right_led, HIGH);
    delay(150);
    digitalWrite(left_led, LOW);
    digitalWrite(right_led, LOW);
    delay(150);
  }

//  Serial.print("left floor: ");
//  Serial.print(FL_floor_avg);
//  Serial.print("right floor: ");
//  Serial.print(FR_floor_avg);
//  Serial.println();
  
  setState(NO_INTERFERENCE);
}

void handleNoInterference() {
  // check back TOFs + grip to see if user wants to sit
    // brake if wants to sit
  // check if any obstacles in front of either of the front TOFs
  // determine if assist turns or short brake are needed

  bool handles_engaged = handlesEngaged();
  Serial.print("handle engaged: ");
  Serial.print(handles_engaged);
  Serial.println();
  
  
  bool braked = false;
  if(!handles_engaged){
    Serial.println("braking! ");
    braked = true;
    Brake();
  }
  while (!handles_engaged) {
    handles_engaged = handlesEngaged();
    Serial.println("keeping brake on ");
    delay(50);
  }
  if (braked) {
    Serial.println("unbraking!  ");
    Unbrake();
  }

  bool right_object = FR_TOF.objectDetected(FR_floor_avg);
  bool left_object = FL_TOF.objectDetected(FL_floor_avg);
  
  if (userWantsToSit()) {
    Serial.println("USER WANTS TO SIT");
    setState(LONG_BRAKE);
    return;
  }
//  if (left_object && right_object) {
//    Serial.println("obstacle on both sides!");
//    setState(SHORT_BRAKE);
//  } else 
  if (left_object) {
    Serial.println("obstacle on left side!");
    setState(ASSIST_RIGHT_TURN);
  } else if (right_object) {
    Serial.println("obstacle on right side!");
    setState(ASSIST_LEFT_TURN);
  }

  // state remains to be no interference
  
//  for (pos = 80; pos <= 170; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(10);                       // waits 15ms for the servo to reach the position
//  }
//  delay(2000);
//  for (pos = 170; pos >= 80; pos -= 1) { // goes from 180 degrees to 0 degrees
//    servo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(10);                       // waits 15ms for the servo to reach the position
//  }
//  delay(2000);
  
  return;
}

void commandMotor(int left_motor_power, int right_motor_power){
  delay(50);
  left_motor.forward(left_motor_power);
  delay(50);
  right_motor.forward(right_motor_power);
  delay(50);
}

void beep(){
    tone(45, NOTE_C7, 1000 / 8);
    delay(750);
    noTone(45);
}

void handleAssistLeftTurn() {
  // turn until we stop seeing the obstacle
  digitalWrite(right_led, HIGH); // right LED on

  right_motor_power = TURN_MOTOR_VALUE;
  left_motor_power = STOP_MOTOR_VALUE;
  while (FR_TOF.objectDetected(FR_floor_avg)) {
    if(FL_TOF.objectDetected(FL_floor_avg)){
      
      right_motor_power = STOP_MOTOR_VALUE;
      commandMotor(left_motor_power, right_motor_power);
      setState(SHORT_BRAKE);
      return;
    }
    tone(45, NOTE_C7, 1000 / 8);
    commandMotor(left_motor_power, right_motor_power);
  }
  delay(500);
  noTone(45);
  digitalWrite(right_led, LOW); // right LED off
  right_motor_power = STOP_MOTOR_VALUE;
  commandMotor(left_motor_power, right_motor_power);
  delay(50);
  Serial.println("Left turn finished!");
  setState(NO_INTERFERENCE);
}

void handleAssistRightTurn() {
  // turn until we stop seeing the obstacle
  digitalWrite(left_led, HIGH); // left LED on

  right_motor_power = STOP_MOTOR_VALUE;
  left_motor_power = TURN_MOTOR_VALUE;
  while (FL_TOF.objectDetected(FL_floor_avg)) {
    if(FR_TOF.objectDetected(FR_floor_avg)){
      left_motor_power = STOP_MOTOR_VALUE;
      commandMotor(left_motor_power, right_motor_power);
      setState(SHORT_BRAKE);
      return;
    }
    tone(45, NOTE_C7, 1000 / 8);
    commandMotor(left_motor_power, right_motor_power);
    delay(50);
    Serial.println("Right turn!");
  }
  delay(500);
  noTone(45);
  digitalWrite(left_led, LOW); // left LED off

  left_motor_power = STOP_MOTOR_VALUE;
  commandMotor(left_motor_power, right_motor_power);
  delay(50);
  
  Serial.println("Right turn finished!");
  setState(NO_INTERFERENCE);
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(1000);
    // LED indicators
    pinMode(left_led, OUTPUT);
    pinMode(right_led, OUTPUT);

    BR_TOF.init();
    BL_TOF.init();
    FL_TOF.init();
    FR_TOF.init();
    delay(100);
    left_motor.init();
    right_motor.init();
    delay(500);
    servo.attach(8);
    delay(500);

    robot_state = INIT;
}

void loop() {  
//   double dist1 = BR_TOF.getDistance();
//   double dist2 = BL_TOF.getDistance();
//   double dist3 = FL_TOF.getDistance();
//   double dist4 = FR_TOF.getDistance();
//   Serial.print("    BR: ");
//   Serial.print(dist1);
//   Serial.print(", BL: ");
//   Serial.print(dist2);
//   Serial.print(", FL: ");
//   Serial.print(dist3);
//   Serial.print(", FR: ");
//   Serial.print(dist4);
//   Serial.print(" cm  ");
//   Serial.println();

 switch (robot_state) {
   case INIT:
     handleInit();
     break;
   case NO_INTERFERENCE:
     handleNoInterference();
     break;
   case ASSIST_LEFT_TURN:
     handleAssistLeftTurn();
     break;
   case ASSIST_RIGHT_TURN:
     handleAssistRightTurn();
     break;
   case SHORT_BRAKE:
     handleShortBrake();
     break;
   case LONG_BRAKE:
     handleLongBrake();
     break;
 }

  delay(50);
}
