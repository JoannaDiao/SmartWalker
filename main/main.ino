// https://electropeak.com/learn/connect-multiple-i2c-devices-to-arduino-using-i2c-multiplexer-tca9548a/
// Uses 3.3V power from Arduino. DO NOT USE 5V output.
#include "../sensors/sensors.h"

VL53L0X Select(int index) {
  if (index == 4) { return sensor4; } // rear right TOF
  if (index == 5) { return sensor5; } // rear left TOF
  if (index == 6) { return sensor6; } // front left TOF
  if (index == 7) { return sensor7; } // front right TOF
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(100);

    Sensors::TOF BR_TOF(4); // rear right TOF
    Sensors::TOF BL_TOF(5); // rear left TOF
    Sensors::TOF FL_TOF(6): // front left TOF
    Sensors::TOF FR_TOF(7); // front right TOF
    BR_TOF.init();
    BL_TOF.init();
    FL_TOF.init();
    FR_TOF.init();
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
  Serial.print(" mm  ");
  Serial.println();

  delay(10);
}
