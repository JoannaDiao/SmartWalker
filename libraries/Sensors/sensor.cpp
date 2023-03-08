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
            // Serial.print("OutOfRange  ");
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
        if (curr_reading == -1)
        {
            return false;
        }
        double delta = floor_value - curr_reading;
        bool changed = abs(delta) >= change_threshold;
        return changed;
    }

    Grip::Grip(uint8_t pin)
    {
        pinToMeasure = pin;
    }

    bool Grip::handleEngaged()
    {
        long curr_reading = readCapacitance();
        // Serial.print("Grip reading: ");
        // Serial.print(curr_reading);
        // Serial.print(" ");

        if (curr_reading >= 6)
        {
            return true;
        }
        return false;
    }

    void Motor::init()
    {
        pinMode(enable_pin_, OUTPUT);
        pinMode(forward_pin_, OUTPUT);
        pinMode(backward_pin_, OUTPUT);

        digitalWrite(forward_pin_, LOW);
        digitalWrite(backward_pin_, LOW);
    }

    void Motor::forward(int speed)
    {
        digitalWrite(forward_pin_, HIGH);
        digitalWrite(backward_pin_, LOW);
        int speed_input = speed / 100.0 * 255;
        analogWrite(enable_pin_, speed_input);
    }

    void Motor::stop()
    {
        analogWrite(enable_pin_, 255); // double check the stopping values for motors
        digitalWrite(forward_pin_, LOW);
        digitalWrite(backward_pin_, LOW);
    }
} // namespace Sensors