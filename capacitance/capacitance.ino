#include <sensors.h>

Sensors::Grip left_grip(44);
Sensors::Grip right_grip(48);

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
  Serial.println();
  bool sitting = false;
  if (bl_dist == -1 || br_dist == -1) {
    sitting = false;
    return sitting;
  } 
  
  if (bl_dist < 25 && br_dist < 25) {
    sitting = true;
  } 
  
  return sitting;
}

bool handleEngaged(int cap_level){
  Serial.print(" handle: ");
  Serial.println(cap_level);
  if(cap_level >= 8){
      return true;
    }
  return false;
}

bool userWantsToSit() {
  // if user is touching the grip && back facing TOF distance decrease below a certain threshold --> user wants to sit down
  //int capacitance = readCapacitance(44);
  
//  bool lg = left_grip.handleEngaged();
//  bool rg = right_grip.handleEngaged();

  bool lg = handleEngaged(readCapacitance(44));
  //if(lg == true){Serial.print(" left gripped ");}
  bool rg = handleEngaged(readCapacitance(48));
  //if(rg == true){Serial.print(" right gripped ");}

  //Serial.println();

  bool gripped = false;

  if( rg && lg){
      gripped = true;
      //Serial.print(" both gripped ");
  }
  
  bool sitting = sittingDetected();
  if (gripped && sitting) {
    return true;
  }
  return false;
}



// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher

uint8_t readCapacitance(int pinToMeasure) {

  // Variables used to translate from Arduino to AVR pin naming

  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;

  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.

  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));

  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);

  uint8_t SREG_old = SREG; //back up the AVR Status Register

  // Prevent the timer IRQ from disturbing our measurement
  // sussy O_O will this cause issues with other sensors?
  noInterrupts();

  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.

  uint8_t cycles = 17;
  if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
  SREG = SREG_old;

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.

  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
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
      //Serial.println("No sitting!");
    }
    delay(100);


}
