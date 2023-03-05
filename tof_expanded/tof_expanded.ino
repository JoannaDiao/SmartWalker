// https://electropeak.com/learn/connect-multiple-i2c-devices-to-arduino-using-i2c-multiplexer-tca9548a/
// Uses 3.3V power from Arduino. DO NOT USE 5V output.
#include <Wire.h>
#include <VL53L0X.h>
#include "MegunoLink.h"
#include "Filter.h"
VL53L0X sensor4;
VL53L0X sensor5;
VL53L0X sensor6;
VL53L0X sensor7;

void TCA9548A(uint8_t bus)  //function of TCA9548A
{
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

VL53L0X Select(int index) {
  if (index == 4) { return sensor4; } // rear right TOF
  if (index == 5) { return sensor5; } // rear left TOF
  if (index == 6) { return sensor6; } // front left TOF
  if (index == 7) { return sensor7; } // front right TOF
}
VL53L0X sensor;

// Create a new exponential filter with a weight of 5 and an initial value of 0. 
long FilterWeight = 20;
ExponentialFilter<long> ADCFilter(FilterWeight, 0);
ExponentialFilter<long> ADCFilter1(FilterWeight, 0);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(1000);

  for (int index = 4; index < 8; index++) {
    TCA9548A(index);
    sensor = Select(index);

    sensor.setTimeout(100);

    if (!sensor.init()) {
      Serial.println("Failed to detect and initialize sensor!");
      while (1) {}
    }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
  }
}

bool changed_significantly(double value) {
    const float change_threshold = 7;   // cm
    const float filter_constant = 0.1;  // this one also
    static float old_value;
    float delta = value - old_value;
    bool changed = abs(delta) >= change_threshold;
    if (changed)
        old_value = value;
    else
        old_value += filter_constant * delta;  // low-pass filter, would detect creeping changes
    return changed;
}

bool sitting_down(double value_left, double value_right) {
    bool sitting = false;
    if(value_left == -1 || value_right == -1){
      sitting = false;
      return sitting;
    } 
    
    if(value_left < 20 && value_right < 20){
      sitting = true;
    } 
    
    return sitting;
}

void loop() {  
//  for (int index = 4; index < 8; index++) {
//    TCA9548A(index);
//    sensor = Select(index);
//    Serial.print("Sensor");
//    Serial.print(index);
//    Serial.print(": ");
//    int reading = sensor.readRangeContinuousMillimeters();
//    
//    if (reading < 8000){
//      ADCFilter.Filter(reading);
//      int filtered_reading = ADCFilter.Current();
//      
//      double cm_reading = filtered_reading/10.0;
//      Serial.print("    ");
//      
//      Serial.print(reading);
//      Serial.print(" cm  ");
//
////      if(changed_significantly(cm_reading)){
////          Serial.print("Jump!");
////      }
////      TimePlot Plot;
////      Plot.SendData("Filtered", filtered_reading);
//      }
//    else {Serial.print("OutOfRange  ");}
//    
//    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
//    delay(100);
//  }

    int index = 5; // 5 is rear left, 6 is front left
    TCA9548A(index);
    sensor = Select(index);
    Serial.print("left");
   // Serial.print(index);
    Serial.print(": ");
    int reading_6 = sensor.readRangeContinuousMillimeters();

    int dist_left = -1;
    
    if (reading_6 < 8000){
      ADCFilter1.Filter(reading_6);
      int filtered_reading_6 = ADCFilter1.Current();
      
      double cm_reading_6 = filtered_reading_6/10.0;
      Serial.print("    ");
      
      Serial.print(cm_reading_6);
      Serial.print(" cm  ");

      dist_left = cm_reading_6;

//      if(changed_significantly(cm_reading_6)){
//          Serial.print("Obstacle in the way!");
//        }
      }
    else {Serial.print("OutOfRange  ");}
    
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    delay(10);
    
    index = 4; // 4 is rear right, 7 is front right 
    TCA9548A(index);
    sensor = Select(index);
    Serial.print("right");
   // Serial.print(index);
    Serial.print(": ");
    int reading = sensor.readRangeContinuousMillimeters();

    int dist_right = -1;
    
    if (reading < 8000){
      ADCFilter.Filter(reading);
      int filtered_reading = ADCFilter.Current();
      
      double cm_reading = filtered_reading/10.0;
      Serial.print("    ");
      
      Serial.print(cm_reading);
      Serial.print(" cm  ");

      dist_right = cm_reading;

//      if(changed_significantly(cm_reading)){
//          Serial.print("Obstacle in the way!");
//        }
      }
    else {Serial.print("OutOfRange  ");}

    if(sitting_down(dist_left, dist_right)){
          Serial.print("Sitting!");
        }

    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    delay(10);
  
  Serial.println();
}
