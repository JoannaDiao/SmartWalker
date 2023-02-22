#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <VL53L0X.h>
#include "MegunoLink.h"
#include "Filter.h"

namespace Sensors {
    class TOF {
    public:
        TOF(uint8_t bus);
        void init();
        int getDistance();
        
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
        int distance_;
    };
} // namespace Sensors

#endif // SENSORS_H