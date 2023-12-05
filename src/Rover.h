#ifndef ESP_ROVER_H
#define ESP_ROVER_H

#include <Servo.h>
#include "UltrasonicSensor.h"
#include "Motor_.h"

class Rover {
public:
  // Default constructor
  Rover();

  // Initialize a rover with the provided components.
  // To attach the servo, see the below functions.
  Rover(UltrasonicSensor, Motor left_motor, Motor right_motor);

  // Attach the servo to a pin.
  void attach_servo(int pin);

  // Attach the servo with minimum and maximum pulse widths.
  void attach_servo(int pin, int min, int max);

  // Detach the servo.
  void detach_servo();

  // Turn both motors to go forward.
  void go_forward();

  enum class Dir { Left, Right };

  // Start turning in the given direction.
  // This function does not block.
  void turn(Dir);

  // Turn 90 degrees.
  // This function blocks for the duration of the turn.
  void turn_90deg(Dir);

  // Stop both motors.
  void stop();

  // Set the "hug" direction (defaults to `Dir::Left`).
  void hug(Dir dir) { hug_dir = dir; }

  // Rotate the servo to look `angle` degrees in the given direction.
  void look(Dir, byte angle = 90);

  // Rotate the servo to look straight ahead.
  void look_ahead() { look(Dir::Left, 0); }

  // Reset the state of the rover.
  void reset();

  // Run one iteration of the rover's main loop.
  void run();

private:
  UltrasonicSensor us;
  Motor left_motor;
  Motor right_motor;
  Servo servo;
  Dir hug_dir = Dir::Left;
};

#endif
