#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(42,44);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
//CapacitiveSensor   cs_4_6 = CapacitiveSensor(46,48);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
//CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil

void setup()                    
{
//     cs_4_2.reset_CS_AutoCal(); 
//     cs_4_6.reset_CS_AutoCal();     
     
     // turn off autocalibrate on channel 1 - just as an example
   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
//   cs_4_6.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);
}

bool handle_engaged(int reading){
  bool handle_engaged = false;
  if(reading> 6000){
    handle_engaged = true;
  } 
  else{
    handle_engaged = false;
  }
  return handle_engaged;
}

void loop()                    
{
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);
//    long total2 =  cs_4_6.capacitiveSensor(20);
    //long total3 =  cs_4_8.capacitiveSensor(30);

    bool grip_engaged = handle_engaged(total1);

    //Serial.print(millis() - start);        // check on performance in milliseconds
    //Serial.print("\t");                    // tab character for debug windown spacing
    Serial.print("  handle_reading:  ");
    Serial.print(total1);                  // print sensor output 1
    Serial.print(", engaged?:  ");
    Serial.print(grip_engaged);                  // print sensor output 2
    Serial.print('\n');     
    //Serial.println(total3);                // print sensor output 3

    delay(1000);                             // arbitrary delay to limit data to serial port 
}
