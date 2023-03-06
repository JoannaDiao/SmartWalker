#include <sensors.h>

Sensors::Grip left_grip(42, 44);
Sensors::TOF BR_TOF(4); // rear right TOF
Sensors::TOF BL_TOF(5); // rear left TOF
Sensors::TOF FL_TOF(6); // front left TOF
Sensors::TOF FR_TOF(7); // front right TOF

bool sittingDetected() {
  double bl_dist = BL_TOF.getDistance();
  double br_dist = BR_TOF.getDistance();
//  Serial.print("BL: ");
//  Serial.print(bl_dist);
//  Serial.print("BR: ");
//  Serial.print(br_dist);
//  Serial.println();
  bool sitting = false;
  if (bl_dist == -1 || br_dist == -1) {
    sitting = false;
    return sitting;
  } 
  
  if (bl_dist < 20 && br_dist < 20) {
    sitting = true;
  } 
  
  return sitting;
}

bool userWantsToSit() {
  // if user is touching the grip && back facing TOF distance decrease below a certain threshold --> user wants to sit down
  bool g = left_grip.handleEngaged();
  bool s = sittingDetected();
  if (g&&s) {
    return true;
  }
  return false;
}


void setup()                    
{
  Serial.begin(9600);
  Wire.begin();
  delay(1000);

  BR_TOF.init();
  BL_TOF.init();
  FL_TOF.init();
  FR_TOF.init();
}

void loop()                    
{
    if (userWantsToSit()) {
      Serial.println("User wants to sit!");
    } else {
//      Serial.println("No sitting!");
    }
//    sittingDetected();
    delay(500);
}
