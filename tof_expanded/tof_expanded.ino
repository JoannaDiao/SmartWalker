// https://electropeak.com/learn/connect-multiple-i2c-devices-to-arduino-using-i2c-multiplexer-tca9548a/
// Uses 3.3V power from Arduino. DO NOT USE 5V output.
#include <sensors.h>

Sensors::TOF BR_TOF(4); // rear right TOF
Sensors::TOF BL_TOF(5); // rear left TOF
Sensors::TOF FL_TOF(6); // front left TOF
Sensors::TOF FR_TOF(7); // front right TOF

double FL_floor_avg;
double FR_floor_avg;

void get_floor_readings(){
  double left_sum = 0;
  double right_sum = 0;
  for (int i = 0; i < 50; i++){
    double left_dist = FL_TOF.getDistance();
    double right_dist = FR_TOF.getDistance();
    
    if( i >= 25){
      right_sum += right_dist;
      left_sum += left_dist;
    }
   }

   FL_floor_avg = left_sum/25;
   FR_floor_avg = right_sum/25;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(5000);

  BR_TOF.init();
  BL_TOF.init();
  FL_TOF.init();
  FR_TOF.init();

  get_floor_readings();
}

void loop() {  
//    int dist1 = BR_TOF.getDistance();
//    int dist2 = BL_TOF.getDistance();
//    int dist3 = FL_TOF.getDistance();
//    int dist4 = FR_TOF.getDistance();
//    Serial.print("    BR: ");
//    Serial.print(dist1);
//    Serial.print(", BL: ");
//    Serial.print(dist2);
//    Serial.print("left avg: ");
//    Serial.print(floor_avg_L);
//    Serial.print("righ avg: ");
//    Serial.print(floor_avg_R);
//    Serial.print(", FL: ");
//    Serial.print(FL_TOF.getDistance());
//    Serial.print(", FR: ");
//    Serial.print(FR_TOF.getDistance());
//    Serial.println();
//    Serial.print(" cm  ");
//    Serial.println();
 
  if (FR_TOF.objectDetected(FR_floor_avg) && FL_TOF.objectDetected(FL_floor_avg)) {
    Serial.println("both!");
  } else if (FR_TOF.objectDetected(FR_floor_avg)) {
    Serial.println("Left only");
  } else if (FL_TOF.objectDetected(FL_floor_avg)) {
    Serial.println("Right only");
  }
  delay(15);
}
