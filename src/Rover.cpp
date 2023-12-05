#include "Rover.h"

// Measure distance (in inches) multiple times and return the median.
// Returns zero if all measurements timed out.
// Based on: https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!timer-median-sketch
static double measure_median(const UltrasonicSensor& us, byte times = 5);

// Return the opposite direction.
static Rover::Dir opposite_dir(Rover::Dir dir) {
  using Dir = Rover::Dir;
  return dir == Dir::Left ? Dir::Right : Dir::Left;
}

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

// Reset the state of the rover.
void Rover::reset() {
  stop();
  look(hug_dir, 45);  // Initially look 45 degrees, between forward and `hug_dir`.
  delay(1300);        // 1.3 seconds to rotate the servo AT MOST
}

// Run one iteration of the rover's main loop.
void Rover::run() {
  // Safe distance between the ultrasonic sensor and the wall
  const double safe_distance = 7.0;

  // Take two measurements roughly in the "hug" direction.
  // The servo should already be rotated 45 degrees.
  double distance_45deg = measure_median(us);

  look(hug_dir);  // Rotate the servo to the 90 degrees direction.
  delay(325);     // 325 ms to rotate 45 degrees (1300 ms / 4 = 325 ms)

  double distance = measure_median(us);

  look(hug_dir, 45);  // Rotate back to the 45 degrees direction.
  delay(325 - 250);   // Moving forward already takes 250 ms; don't waste time.

  if (distance == 0.0 && safe_distance == 0.0) {
    // If both measurements timed out, give up.
    return;
  } else if (distance == 0.0) {
    // If only the second measurement timed out, use `distance_45deg`.
    distance = distance_45deg;
  } else if (distance_45deg != 0.0) {
    // If both measurements are valid, use the minimum.
    // It's better to assume that the rover is too close to any walls rather
    // than assume that the rover is at a safe distance.
    distance = distance < distance_45deg ? distance : distance_45deg;
  }

  // For debugging purposes:
  Serial.println(distance);

  if (distance < safe_distance - 0.5) {
    // Too close: turn away from wall.
    turn(opposite_dir(hug_dir));
    delay(90);
    stop();
  } else if (distance > safe_distance + 0.5) {
    // Too far: turn towards wall.
    turn(hug_dir);
    delay(90);
    stop();
  }

  // Blindly go forward for 250 ms.
  // TODO: Check that there is no wall directly in front of the rover.
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
