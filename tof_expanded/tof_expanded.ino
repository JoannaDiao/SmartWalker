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

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(100);

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

void loop() {  
  for (int index = 4; index < 8; index++) {
    TCA9548A(index);
    sensor = Select(index);
    Serial.print("Sensor");
    Serial.print(index);
    Serial.print(": ");
    int reading = sensor.readRangeContinuousMillimeters();
    
    if (reading < 8000){
      ADCFilter.Filter(reading);
      int filtered_reading = ADCFilter.Current();
      Serial.print("    ");
      Serial.print(filtered_reading);
      Serial.print(" mm  ");
      TimePlot Plot;
      Plot.SendData("Raw", reading);
      Plot.SendData("Filtered", filtered_reading);
      }
    else {Serial.print("OutOfRange  ");}
    
    if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    delay(10);
  }
  Serial.println();
}
