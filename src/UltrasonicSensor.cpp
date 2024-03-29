#include "UltrasonicSensor.h"

// The reciprocal of the speed of sound in air at 20 °C, in µs per inch:
//     1,126 ft/s = 0.013512 in/µs -> 74.0 µs/in
constexpr double inv_sound_us_per_in = 74.0;

// The reciprocal of the speed of sound in air at 20 °C, in µs per centimeter:
//     343.3 m/s = 0.03433 cm/µs -> 29.1 µs/cm
constexpr double inv_sound_us_per_cm = 29.1;

// Convert duration in microseconds to distance in inches.
double ultrasonic_convert(unsigned long duration) {
  if (duration == 0) return 0.0; // Timeout
  return duration / inv_sound_us_per_in;
}

// Convert duration in microseconds to distance in centimeters.
double ultrasonic_convert_cm(unsigned long duration) {
  if (duration == 0) return 0.0; // Timeout
  return duration / inv_sound_us_per_cm;
}

// Default initialize the sensor without changing any pin modes.
UltrasonicSensor::UltrasonicSensor() = default;

// Initialize a three-pin ultrasonic sensor.
UltrasonicSensor::UltrasonicSensor(byte signal_pin)
  : UltrasonicSensor{ signal_pin, signal_pin }
{}

// Initialize a four-pin ultrasonic sensor.
UltrasonicSensor::UltrasonicSensor(byte trigger_pin, byte echo_pin)
  : trigger_pin{ trigger_pin }
  , echo_pin{ echo_pin }
{}

// Measure the time it took for an echo to bounce off of something,
// in microseconds. This is half the round-trip time.
// Returns zero if the function timed out.
unsigned long UltrasonicSensor::measure_time() const {
  // We want to detect objects up to 13 feet away.
  const unsigned long max_distance_ft = 13;
  const unsigned long max_distance_in = max_distance_ft * 12;
  const unsigned long max_duration_us = max_distance_in * inv_sound_us_per_in * 2;

  // We don't want to be interrupted while measuring.
  noInterrupts();

  // Before triggering, make sure the echo pin starts low.
  pinMode(echo_pin, OUTPUT);
  digitalWrite(echo_pin, LOW);

  // Prepare to trigger the sensor.
  pinMode(trigger_pin, OUTPUT);

  // Give a short low pulse beforehand to ensure a clean high pulse.
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(5);

  // Send a 10 µs high pulse.
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  // 1. Wait for the echo pin to go high.
  // 2. Wait for the echo pin to return to low.
  // 3. Return the length of the pulse, which is how long it took
  //    the echo pin to return to low (in microseconds).
  // Timeout after `max_duration_us`.
  // NOTE: The Arduino documentation is misleading. The timeout includes
  // waiting for the previous pulse to end, the next pulse to start, and the
  // next pulse to end [Source: 'wiring_pulse.S' in ArduinoCore-avr repository].
  pinMode(echo_pin, INPUT);
  unsigned long duration = pulseIn(echo_pin, HIGH, max_duration_us);

  // Now we can safely re-enable interrupts.
  interrupts();

  if (duration == 0)
    return 0; // Timeout

  // Divide the round-trip time by two to get the one-way time.
  // Round up to avoid returning zero, which would indicate timeout.
  return (duration + 1) / 2;
}

// Return the measured distance in inches (or zero on timeout).
double UltrasonicSensor::measure_distance() const {
  return ultrasonic_convert(measure_time());
}

// Return the measured distance in centimeters (or zero on timeout).
double UltrasonicSensor::measure_distance_cm() const {
  return ultrasonic_convert_cm(measure_time());
}
