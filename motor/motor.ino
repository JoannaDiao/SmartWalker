// https://microcontrollerslab.com/l298n-dc-motor-driver-module-arduino-tutorial/
// If using USB, DO NOT CONNECT battery 12V to the board
// If using battery, connect EVERYTHING before switching on power. 
int ENA = 7;
int IN1 = 6;
int IN2 = 5;
int IN3 = 4;
int IN4 = 3;
int ENB = 2;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  // motor, direction, speed
  // motor 1: direct 1 -> fwd, direct 2 -> bckwd
  // motor 2: direct 2 -> fwd, direct 1 -> bckwd
  turn_right(60);
}

void turn_right(int speed) {
  if(speed > 60){ speed = 60;}
  
  setDirectionSpeed(1, 2, 30); // right backwards
  setDirectionSpeed(2, 2, speed); // left 
  delay(100);
}

void turn_left() {
  
  setDirectionSpeed(1, 1, 60); // right
  setDirectionSpeed(2, 2, 0); // left 
  delay(100);
}

void go_straight(int speed) {
  if(speed > 60){ speed = 60;}
  
  setDirectionSpeed(1, 1, speed); // right
  setDirectionSpeed(2, 2, speed); // left 
  delay(100);
}

void setDirectionSpeed(int motor, int dir, int speedPercentage) {
  int pinA;
  int pinB;
  int pinEnable;
  if (motor == 1){
    pinA = IN1;
    pinB = IN2;
    pinEnable = ENA;
  }
  else{
    pinA = IN3;
    pinB = IN4;
    pinEnable = ENB;
  }
  if (dir == 0){
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }else if (dir == 1){
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  }else{
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
  }

  //max speed at 255
  int speed = speedPercentage / 100.0 * 255;
  analogWrite(pinEnable, speed); 

  delay(100);
}
