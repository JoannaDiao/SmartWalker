// https://electropeak.com/learn/connect-multiple-i2c-devices-to-arduino-using-i2c-multiplexer-tca9548a/
// Uses 3.3V power from Arduino. DO NOT USE 5V output.
#include <sensors.h>

Sensors::TOF BR_TOF(4); // rear right TOF
Sensors::TOF BL_TOF(5); // rear left TOF
Sensors::TOF FL_TOF(6); // front left TOF
Sensors::TOF FR_TOF(7); // front right TOF


void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(5000);

  BR_TOF.init();
  BL_TOF.init();
  FL_TOF.init();
  FR_TOF.init();
  
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
//    Serial.print(", FL: ");
//    Serial.print(dist3);
//    Serial.print(", FR: ");
//    Serial.print(dist4);
//    Serial.print(" cm  ");
//    Serial.println();
  if (FR_TOF.objectDetected() && FL_TOF.objectDetected()) {
    Serial.println("both!");
  } else if (FR_TOF.objectDetected()) {
    Serial.println("Left only");
  } else if (FL_TOF.objectDetected()) {
    Serial.println("Right only");
  }
  delay(15);
}
