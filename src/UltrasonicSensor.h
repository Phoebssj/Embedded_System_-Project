#ifndef ESP_ULTRASONIC_SENSOR_H
#define ESP_ULTRASONIC_SENSOR_H

#include <Arduino.h>

// Convert duration in microseconds to distance in inches.
double ultrasonic_convert(unsigned long duration);

// Convert duration in microseconds to distance in centimeters.
double ultrasonic_convert_cm(unsigned long duration);

class UltrasonicSensor {
public:
  // Default initialize the sensor without changing any pin modes.
  UltrasonicSensor();

  // Initialize a three-pin ultrasonic sensor.
  UltrasonicSensor(byte signal_pin);

  // Initialize a four-pin ultrasonic sensor.
  UltrasonicSensor(byte trigger_pin, byte echo_pin);

  // Measure the time it took for an echo to bounce off of something,
  // in microseconds. This is half the round-trip time.
  // Returns zero if the function timed out.
  unsigned long measure_time() const;

  // Return the measured distance in inches (or zero on timeout).
  double measure_distance() const;

  // Return the measured distance in centimeters (or zero on timeout).
  double measure_distance_cm() const;

private:
  byte trigger_pin = 0;
  byte echo_pin = 0;
};

#endif
