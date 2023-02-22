#include "sensors.h"

namespace Sensors {
    TOF::TOF(uint8_t bus) {
        loxAddress = bus;
    }

    TOF::init() {
        TCA9548A(loxAddress);
        sensor.setTimeout(100);
        if (!sensor.init()) {
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {} // either terminate or do something else here
        }
        ADCFilter(FilterWeight, 0);

        // Start continuous back-to-back mode (take readings as
        // fast as possible).  To use continuous timed mode
        // instead, provide a desired inter-measurement period in
        // ms (e.g. sensor.startContinuous(100)).
        sensor.startContinuous();
    }

    int TOF::getDistance() {
        int reading = sensor.readRangeContinuousMillimeters();
        if (reading < 8000) {
            ADCFilter.Filter(reading);
            int filtered_reading = ADCFilter.Current();
            //   Serial.print("    ");
            //   Serial.print(filtered_reading);
            //   Serial.print(" mm  ");
            //   TimePlot Plot;
            //   Plot.SendData("Raw", reading);
            //   Plot.SendData("Filtered", filtered_reading);
            distance_ = filtered_reading;
        }
        else {
            Serial.print("OutOfRange  ");
            distance_ = -1;
        }
        
        if (sensor.timeoutOccurred()) { 
            Serial.print("TIMEOUT");
            distance_ = -1;
        }

        return distance_;
    }
} // namespace Sensors