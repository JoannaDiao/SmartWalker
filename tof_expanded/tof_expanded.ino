// https://electropeak.com/learn/connect-multiple-i2c-devices-to-arduino-using-i2c-multiplexer-tca9548a/
// Uses 3.3V power from Arduino. DO NOT USE 5V output.
#include <sensors.h>

Sensors::TOF BR_TOF(4); // rear right TOF
Sensors::TOF BL_TOF(5); // rear left TOF
Sensors::TOF FL_TOF(6); // front left TOF
Sensors::TOF FR_TOF(7); // front right TOF

double FL_floor_avg;
double FR_floor_avg;

int left_led = 49;
int right_led = 47;

int i = 0;

void get_floor_readings(){
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

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(1000);

  pinMode(left_led, OUTPUT);
  pinMode(right_led, OUTPUT);

  BR_TOF.init();
  BL_TOF.init();
  FL_TOF.init();
  FR_TOF.init();

  get_floor_readings();

  for (int i = 0; i < 2; i++) {
    digitalWrite(left_led, HIGH);
    digitalWrite(right_led, HIGH);
    delay(150);
    digitalWrite(left_led, LOW);
    digitalWrite(right_led, LOW);
    delay(150);
  }
}

void loop() {  
    int dist1 = BR_TOF.getDistance();
    int dist2 = BL_TOF.getDistance();
    int dist3 = FL_TOF.getDistance();
    int dist4 = FR_TOF.getDistance();
    Serial.print("    BR: ");
    Serial.print(dist1);
    Serial.print(", BL: ");
    Serial.print(dist2);

    Serial.print(", FL: ");
    Serial.print(dist3);
    Serial.print(", FR: ");
    Serial.print(dist4);
    Serial.println();
    Serial.print(" cm  ");
    Serial.println();
  if (i == 0) {
    i++;
    Serial.print("FL_floor_avg: ");
    Serial.print(FL_floor_avg);
    Serial.print("FR_floor_avg: ");
    Serial.print(FR_floor_avg);
  }
//  double fr = FR_TOF.getDistance();
//  double fl = FL_TOF.getDistance();
//  Serial.print(" FR: ");
//  Serial.print(fr);
//  Serial.print(" FL: ");
//  Serial.print(fl);
//  Serial.println();
 
  if (FR_TOF.objectDetected(FR_floor_avg) && FL_TOF.objectDetected(FL_floor_avg)) {
    Serial.println("both!");
  } else if (FR_TOF.objectDetected(FR_floor_avg)) {
    Serial.println("Left only");
  } else if (FL_TOF.objectDetected(FL_floor_avg)) {
    Serial.println("Right only");
  }
  delay(15);
}
