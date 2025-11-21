#pragma once
#include <Arduino.h>

#define SENSOR_FILTER_COUNTER_MAX 3

#define MIN_SONIC_DISTANCE 3
#define MAX_SONIC_DISTANCE 50
#define SONIC_MEASURE_INTERVAL_MS 60
#define SONIC_PULSE_TIMEOUT_US 30000

class Sonic
{
public:
    Sonic(uint8_t pin_echo, uint8_t pin_trigger) : _pin_echo(pin_echo), _pin_trigger(pin_trigger)
    {
        pinMode(pin_echo, INPUT);
        pinMode(pin_trigger, OUTPUT);
        digitalWrite(pin_trigger, LOW);
    }

    float get_distance()
    {
        const unsigned long now = millis();
        if (now - _last_measurement_ms < SONIC_MEASURE_INTERVAL_MS)
        {
            return _last_distance;
        }
        _last_measurement_ms = now;

        digitalWrite(_pin_trigger, LOW);
        delayMicroseconds(2);
        digitalWrite(_pin_trigger, HIGH);
        delayMicroseconds(10);
        digitalWrite(_pin_trigger, LOW);

        const unsigned long duration = pulseIn(_pin_echo, HIGH, SONIC_PULSE_TIMEOUT_US);
        if (duration == 0)
        {
            return _last_distance;
        }

        float distance = duration / 58.0f; // in cm
        distance = constrain(distance, MIN_SONIC_DISTANCE, MAX_SONIC_DISTANCE);
        _sensor_values[_sensor_values_counter] = distance;
        if (++_sensor_values_counter >= SENSOR_FILTER_COUNTER_MAX)
            _sensor_values_counter = 0;

        // Averaging
        float sum = 0;
        for (uint8_t i = 0; i < SENSOR_FILTER_COUNTER_MAX; i++)
        {
            sum += _sensor_values[i];
        }
        float averaged = sum / SENSOR_FILTER_COUNTER_MAX;

        // Median filter
        _median_counter = 0;
         if (++_median_counter >= SENSOR_FILTER_COUNTER_MAX)
            _median_counter = 0;

        _median_values[_sensor_values_counter] = averaged;

        const float a = _median_values[0];
        const float b = _median_values[1];
        const float c = _median_values[2];
        float median;
        if ((a <= b && b <= c) || (c <= b && b <= a))
            median = b;
        else if ((b <= a && a <= c) || (c <= a && a <= b))
            median = a;
        else
            median = c;

        return median;
    }

private:
    uint8_t _pin_echo, _pin_trigger;
    float _sensor_values[SENSOR_FILTER_COUNTER_MAX];
    uint8_t _sensor_values_counter = 0;
    float _last_distance = MAX_SONIC_DISTANCE;
    unsigned long _last_measurement_ms = 0;
    float _median_values[3] = {MAX_SONIC_DISTANCE, MAX_SONIC_DISTANCE, MAX_SONIC_DISTANCE};
    uint8_t _median_counter = 0;
};
