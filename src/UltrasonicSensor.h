#ifndef ESP_ULTRASONIC_SENSOR_H
#define ESP_ULTRASONIC_SENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
public:
  // Initialize a three-pin ultrasonic sensor.
  UltrasonicSensor(byte signal_pin);

  // Initialize a four-pin ultrasonic sensor.
  UltrasonicSensor(byte trigger_pin, byte echo_pin);

  // Measure the time it took for an echo to bounce off of something,
  // in microseconds. This is half the round-trip time.
  unsigned long measure_time() const;

  // Return the measured distance in inches.
  double measure_distance() const;

  // Return the measured distance in centimeters.
  double measure_distance_cm() const;

private:
  byte trigger_pin;
  byte echo_pin;
};

#endif
