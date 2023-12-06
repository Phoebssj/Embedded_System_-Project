#include "Rover.h"

// Measure distance (in inches) multiple times and return the median.
// Returns zero if all measurements timed out.
// Based on: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!timer-median-sketch
static double measure_median(const UltrasonicSensor& us, byte times = 10);

// Default constructor
Rover::Rover() = default;

// Initialize a rover with the provided components.
Rover::Rover(UltrasonicSensor us, Motor left_motor, Motor right_motor)
  : us{ us }, left_motor{ left_motor }, right_motor{ right_motor } {}

// Attach the servo to a pin.
void Rover::attach_servo(int pin) {
  // NOTE: Ignoring return value
  (void)servo.attach(pin);
}

// Attach the servo with minimum and maximum pulse widths.
void Rover::attach_servo(int pin, int min, int max) {
  // NOTE: Ignoring return value
  (void)servo.attach(pin, min, max);
}

// Detach the servo.
void Rover::detach_servo() {
  servo.detach();
}

// Turn both motors to go forward.
void Rover::go_forward() {
  left_motor.setSpeed(150);
  left_motor.move(Motor::Forward);
  right_motor.setSpeed(150);
  right_motor.move(Motor::Forward);
}

// Turn both motors to go reverse.
void Rover::go_reverse() {
  left_motor.setSpeed(150);
  left_motor.move(Motor::Reverse);
  right_motor.setSpeed(150);
  right_motor.move(Motor::Reverse);
}

// Start turning in the given direction.
// This function does not block.
void Rover::turn(Dir dir) {
  switch (dir) {
    case Dir::Left:
      left_motor.setSpeed(165);
      left_motor.move(Motor::Reverse);
      right_motor.setSpeed(150);
      right_motor.move(Motor::Forward);
      break;
    case Dir::Right:
      left_motor.setSpeed(150);
      left_motor.move(Motor::Forward);
      right_motor.setSpeed(165);
      right_motor.move(Motor::Reverse);
      break;
  }
}

// Turn 90 degrees.
// This function blocks for the duration of the turn.
void Rover::turn_90deg(Dir dir) {
  turn(dir);
  delay(300);
  stop();
}

// Stop both motors.
void Rover::stop() {
  left_motor.move(Motor::Stop);
  right_motor.move(Motor::Stop);
}

// Rotate the servo to look `angle` degrees in the given direction.
void Rover::look(Dir dir, byte angle) {
  switch (dir) {
    case Dir::Left:
      angle = 90 + angle;
      break;
    case Dir::Right:
      angle = 90 - angle;
      break;
  }
  servo.write(angle);
}

// It takes 1.3 seconds or 1,300 milliseconds for the servo to rotate 180 degrees.
const int servo_delay_180deg_ms = 1300;
const int servo_delay_90deg_ms = servo_delay_180deg_ms / 2;
const int servo_delay_45deg_ms = servo_delay_90deg_ms / 2;

// The walls of the course are 28 inches apart.
const double course_width_in = 28.0;

// A value chosen to represent "large" distances. If the ultrasonic sensor
// timed out, we assume all obstacles/walls are at least this far away.
const double timeout_distance = 70.0;

// Reset the state of the rover.
void Rover::reset() {
  stop();
  look(Dir::Left);
  delay(servo_delay_180deg_ms);
  // The length of the course is unknown, so guess:
  avg_ahead_dist = timeout_distance;
  // The rover begins halfway between the walls.
  avg_left_dist = course_width_in / 2.0;
  avg_right_dist = course_width_in / 2.0;
}

// Run one iteration of the rover's main loop.
void Rover::run() {
  // We can get away waiting 250 ms less since the rover always moves forward
  // for 250 ms each iteration.
  delay(servo_delay_180deg_ms - 250);
  double left_dist = measure_median(us);

  look_ahead();
  delay(servo_delay_90deg_ms);
  double ahead_dist = measure_median(us);

  look(Dir::Right);
  delay(servo_delay_90deg_ms);
  double right_dist = measure_median(us);

  // Rotate the servo left in the meantime.
  look(Dir::Left);

  // Consider timeouts to be large distances.
  if (left_dist == 0.0) left_dist = timeout_distance;
  if (ahead_dist == 0.0) ahead_dist = timeout_distance;
  if (right_dist == 0.0) right_dist = timeout_distance;

  // Compute a weighted average to smooth out deviations in measurements over time.
  // Only average if the measurement didn't timeout.
  const double weight = 0.2;
  avg_left_dist = weight * avg_left_dist + (1.0 - weight) * left_dist;
  avg_ahead_dist = weight * avg_ahead_dist + (1.0 - weight) * ahead_dist;
  avg_right_dist = weight * avg_right_dist + (1.0 - weight) * right_dist;

  // The rover should not get closer than `unsafe_dist_in` inches to the walls.
  // This is half of the course width, minus a few inches to provide some leeway.
  // That means the rover should stay roughly in the middle of the course.
  const double unsafe_dist_in = course_width_in / 2.0 - 3.0;

  if (avg_left_dist < unsafe_dist_in) {
    // The rover is too close to the left wall.
    // Turn slightly right.
    turn(Dir::Right);
    delay(90);
    stop();
  }

  if (avg_right_dist < unsafe_dist_in) {
    // The rover is too close to the right wall.
    // Turn slightly left.
    turn(Dir::Left);
    delay(90);
    stop();
  }

  if (avg_ahead_dist < unsafe_dist_in) {
    // There's a wall ahead of us.
    // Turn in the direction that doesn't have a wall.
    if (avg_left_dist < avg_right_dist) {
      // If the left wall is closer, consider the right direction safe.
      turn_90deg(Dir::Right);
    } else {
      // Otherwise consider the left direction safe.
      turn_90deg(Dir::Left);
    }

    // Now that we've turned around, our previous measurements are invalid.
    // So, reset the state of the rover.
    reset();
  }

  // It should be safe to move forward a few inches.
  go_forward();
  delay(250);
  stop();
}

// Sort the array using insertion sort.
static void sort(unsigned long* numbers, byte length) {
  // For an array of length N, perform N passes of insertion sort.
  for (byte pass = 0; pass < length; ++pass) {
    // The subarray `numbers[0 .. pass]` (not including `numbers[pass]`) is
    // sorted. Insert `numbers[pass]` into that subarray.
    unsigned long to_insert = numbers[pass];
    byte i = pass;
    for (; i > 0 && numbers[i - 1] > to_insert; --i)
      numbers[i] = numbers[i - 1];
    numbers[i] = to_insert;
    // Now `numbers[0 .. pass + 1]` is sorted.
  }
}

// Measure distance (in inches) multiple times and return the median.
// Returns zero if all measurements timed out.
// Based on: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!timer-median-sketch
static double measure_median(const UltrasonicSensor& us, byte times) {
  // Wow! VLAs work! (The length of this array is not a compile-time constant.)
  unsigned long samples[times];
  for (byte i = 0; i < times;) {
    unsigned long duration = us.measure_time();
    if (duration == 0) {
      // Measurement timed out, so we'll ignore that sample.
      --times;
      continue;
    }
    // Append the duration.
    samples[i++] = duration;
    // Delay to avoid cross-sensor echo/crosstalk.
    // NOTE: Does this apply to our use case?
    delay(33);
  }
  // If all measurements timed out, return 0.
  if (times == 0) return 0.0;
  sort(samples, times);
  // Convert and return the median (middle element).
  return ultrasonic_convert(samples[times / 2]);
}
