#ifndef SENSORS_H
#define SENSORS_H

// Libraries for TOF sensors
#include <Wire.h>
#include <VL53L0X.h>
#include "MegunoLink.h"
#include "Filter.h"
// Library for hand grip
#include <CapacitiveSensor.h>

namespace Sensors {
    class TOF {
    public:
        TOF(uint8_t bus);
        void init();
        double getDistance();
        bool objectDetected();
        
    private:
        void TCA9548A(uint8_t bus)  // function of TCA9548A
        {
            Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
            Wire.write(1 << bus);          // send byte to select bus
            Wire.endTransmission();
        }

        uint8_t loxAddress;
        VL53L0X sensor;
        long FilterWeight = 20;
        ExponentialFilter<long> ADCFilter{ExponentialFilter<long>(FilterWeight, 0)};
        double distance_;
        double prev_distance_ = -1;
        const double change_threshold = 7.0;   // cm
    };

    class Grip {
    public:
        Grip(uint8_t pin1, uint8_t pin2) {
            grip_ = CapacitiveSensor(pin1, pin2);
            // turn off autocalibrate on channel 1 - just as an example
            grip_.set_CS_AutocaL_Millis(0xFFFFFFFF);
        }
        bool handleEngaged();

    private:
        inline long getReading() {
            return grip_.capacitiveSensor(n_measurements_);
        }
        CapacitiveSensor grip_ = CapacitiveSensor(0, 0);
        int n_measurements_ = 30;
    };
} // namespace Sensors

#endif // SENSORS_H