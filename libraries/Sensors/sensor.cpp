#include "sensors.h"

namespace Sensors
{
    TOF::TOF(uint8_t bus)
    {
        loxAddress = bus;
    }

    void TOF::init()
    {
        TCA9548A(loxAddress);
        sensor.setTimeout(100);
        int retry = 0;
        while (!sensor.init() && retry < 5)
        {
            Serial.println("Failed to detect and initialize sensor!");
            retry++;
            delay(100);
        }

        // Start continuous back-to-back mode (take readings as
        // fast as possible).  To use continuous timed mode
        // instead, provide a desired inter-measurement period in
        // ms (e.g. sensor.startContinuous(100)).
        sensor.startContinuous();
    }

    double TOF::getDistance()
    {
        TCA9548A(loxAddress);
        int reading = sensor.readRangeContinuousMillimeters();
        if (reading < 8000)
        {
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
        else
        {
            Serial.print("OutOfRange  ");
            distance_ = -1;
        }

        if (sensor.timeoutOccurred())
        {
            Serial.print("TIMEOUT");
            distance_ = -1;
        }

        return distance_;
    }

    bool TOF::objectDetected(double floor_value)
    {
        double curr_reading = getDistance();
        double delta = floor_value - curr_reading;
        bool changed = abs(delta) >= change_threshold;
        return changed;
    }

    bool Grip::handleEngaged()
    {
        long curr_reading = getReading();
        // Serial.print("Grip reading: ");
        // Serial.print(curr_reading);
        // Serial.print(" ");
        bool handle_engaged = false;
        if (curr_reading > 6000)
        {
            handle_engaged = true;
            return handle_engaged;
        }
        return handle_engaged;
    }
} // namespace Sensors