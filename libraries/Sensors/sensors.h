#ifndef SENSORS_H
#define SENSORS_H

// Libraries for TOF sensors
#include <Wire.h>
#include <VL53L0X.h>
#include "MegunoLink.h"
#include "Filter.h"
// Library for hand grip
#include <CapacitiveSensor.h>

namespace Sensors
{
    class TOF
    {
    public:
        TOF(uint8_t bus);
        void init();
        double getDistance();
        bool objectDetected(double floor_value);

    private:
        void TCA9548A(uint8_t bus) // function of TCA9548A
        {
            Wire.beginTransmission(0x70); // TCA9548A address is 0x70
            Wire.write(1 << bus);         // send byte to select bus
            Wire.endTransmission();
        }

        uint8_t loxAddress;
        VL53L0X sensor;
        long FilterWeight = 20;
        ExponentialFilter<long> ADCFilter{ExponentialFilter<long>(FilterWeight, 0)};
        double distance_;
        double prev_distance_ = -1;
        const double change_threshold = 7.0; // cm
    };

    class Grip
    {
    public:
        Grip(uint8_t pin);
        bool handleEngaged();

    private:
        uint8_t readCapacitance()
        {

            // Variables used to translate from Arduino to AVR pin naming
            volatile uint8_t *port;
            volatile uint8_t *ddr;
            volatile uint8_t *pin;

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
            *ddr |= bitmask;
            delay(1);

            uint8_t SREG_old = SREG; // back up the AVR Status Register

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
            if (*pin & bitmask)
            {
                cycles = 0;
            }
            else if (*pin & bitmask)
            {
                cycles = 1;
            }
            else if (*pin & bitmask)
            {
                cycles = 2;
            }
            else if (*pin & bitmask)
            {
                cycles = 3;
            }
            else if (*pin & bitmask)
            {
                cycles = 4;
            }
            else if (*pin & bitmask)
            {
                cycles = 5;
            }
            else if (*pin & bitmask)
            {
                cycles = 6;
            }
            else if (*pin & bitmask)
            {
                cycles = 7;
            }
            else if (*pin & bitmask)
            {
                cycles = 8;
            }
            else if (*pin & bitmask)
            {
                cycles = 9;
            }
            else if (*pin & bitmask)
            {
                cycles = 10;
            }
            else if (*pin & bitmask)
            {
                cycles = 11;
            }
            else if (*pin & bitmask)
            {
                cycles = 12;
            }
            else if (*pin & bitmask)
            {
                cycles = 13;
            }
            else if (*pin & bitmask)
            {
                cycles = 14;
            }
            else if (*pin & bitmask)
            {
                cycles = 15;
            }
            else if (*pin & bitmask)
            {
                cycles = 16;
            }

            // End of timing-critical section; turn interrupts back on if they were on before, or leave them off if they were off before
            SREG = SREG_old;

            // Discharge the pin again by setting it low and output
            //  It's important to leave the pins low if you want to
            //  be able to touch more than 1 sensor at a time - if
            //  the sensor is left pulled high, when you touch
            //  two sensors, your body will transfer the charge between
            //  sensors.

            *port &= ~(bitmask);
            *ddr |= bitmask;

            return cycles;
        }
        int pinToMeasure;
    };

    class Motor
    {
    public:
        Motor(int IN1, int IN2, int EN)
        {
            forward_pin_ = IN1;
            backward_pin_ = IN2;
            enable_pin_ = EN;
        }
        void init();
        void forward(int speed);
        void stop();

    private:
        // void setDirectionSpeed();
        int forward_pin_;
        int backward_pin_;
        int enable_pin_;
    };
} // namespace Sensors

#endif // SENSORS_H