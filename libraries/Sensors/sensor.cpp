#include "sensors.h"

namespace Sensors {
    TOF::TOF(uint8_t bus) {
        loxAddress = bus;
    }

    void TOF::init() {
        TCA9548A(loxAddress);
        sensor.setTimeout(100);
        if (!sensor.init()) {
            Serial.println("Failed to detect and initialize sensor!");
            while (1) {} // either terminate or do something else here
        }

        // Start continuous back-to-back mode (take readings as
        // fast as possible).  To use continuous timed mode
        // instead, provide a desired inter-measurement period in
        // ms (e.g. sensor.startContinuous(100)).
        sensor.startContinuous();
    }

    double TOF::getDistance() {
        TCA9548A(loxAddress);
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
            distance_ = filtered_reading / 10.0;
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

    bool TOF::objectDetected() {
        double curr_reading = getDistance();
        const double filter_constant = 0.1;  // cm
        if (prev_distance_ == -1) {
            prev_distance_ = curr_reading;
            return false;
        }
        double delta = curr_reading - prev_distance_;
        bool changed = abs(delta) >= change_threshold;
        if (changed)
            prev_distance_ = curr_reading;
        else
            prev_distance_ += filter_constant * delta;  // low-pass filter, would detect creeping changes
        return changed;
    }
} // namespace Sensors