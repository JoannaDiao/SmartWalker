#include <sensors.h>

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

Sensors::TOF BR_TOF(4); // rear right TOF
Sensors::TOF BL_TOF(5); // rear left TOF
Sensors::TOF FL_TOF(6); // front left TOF
Sensors::TOF FR_TOF(7); // front right TOF

const int TURN_MOTOR_VALUE = 60;
const int STOP_MOTOR_VALUE = 0;

void setState(robot_state_t new_state) {
  robot_state = new_state;
}

bool userWantsToSit() {
  // if user is touching the grip && back facing TOF distance decrease below a certain threshold
    // return true
  // else
    // return false
}

void brake() {
  // 
}

void handleInit() {
  BR_TOF.init();
  BL_TOF.init();
  FL_TOF.init();
  FR_TOF.init();
  // TODO: check to see if the TOFs are properly initialized
  setState(NO_INTERFERENCE);
}

void handleNoInterference() {
  // check back TOFs + grip to see if user wants to sit
    // brake if wants to sit
  // check if any obstacles in front of either of the front TOFs
  // determine if assist turns or short brake are needed
  if (userWantsToSit()) {
    setState(LONG_BRAKE);
    return;
  }
  if (FL_TOF.objectDetected() && FR_TOF.objectDetected()) {
    setState(SHORT_BRAKE);
  } else if (FL_TOF.objectDetected()) {
    setState(ASSIST_RIGHT_TURN);
  } else if (FR_TOF.objectDetected()) {
    setState(ASSIST_LEFT_TURN);
  }
  // state remains to be no interference
  return;
}

void handleAssistLeftTurn() {
  // turn until we stop seeing the obstacle
  right_motor_power = TURN_MOTOR_VALUE;
  left_motor_power = STOP_MOTOR_VALUE;
  while (FR_TOF.objectDetected()) {
    commandMotor(left_motor_power, right_motor_power);
    Serial.print("Left turn!");
  }
  setState(NO_INTERFERENCE);
}

void handleAssistRightTurn() {
  // turn until we stop seeing the obstacle
  right_motor_power = STOP_MOTOR_VALUE;
  left_motor_power = TURN_MOTOR_VALUE;
  while (FL_TOF.objectDetected()) {
    commandMotor(left_motor_power, right_motor_power);
    Serial.print("Right turn!");
  }
  setState(NO_INTERFERENCE);
}

void handleShortBrake() {
  brake();
  delay(5000);
}

void handleLongBrake() {
  brake();
  delay(10000);
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(100);
}

void loop() {  
  // int dist1 = BR_TOF.getDistance();
  // int dist2 = BL_TOF.getDistance();
  // int dist3 = FL_TOF.getDistance();
  // int dist4 = FR_TOF.getDistance();
  // Serial.print("    BR: ");
  // Serial.print(dist1);
  // Serial.print(", BL: ");
  // Serial.print(dist2);
  // Serial.print(", FL: ");
  // Serial.print(dist3);
  // Serial.print(", FR: ");
  // Serial.print(dist4);
  // Serial.print(" mm  ");
  // Serial.println();

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
    case BRAKE:
      handleBrake();
      break;
  }

  delay(50);
}