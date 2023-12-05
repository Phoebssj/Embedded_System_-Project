#include <Arduino.h>
#include <HardwareSerial.h>
#include "config.h"
#include "UltrasonicSensor.h"
#include "Motor_.h"
#include "Rover.h"

// Explicitly declare `Serial` to get nice editor suggestions.
extern HardwareSerial Serial;

// Declare and default initialize the rover.
// This does nothing except waste a global servo index, lol:
// https://github.com/arduino-libraries/Servo/blob/f36eb392553fc5e89/src/avr/Servo.cpp#L226
Rover rover;

void setup() {
  Serial.begin(9600);

  // Initialize each of the components.
  // These can safely be copied into the rover object.
  UltrasonicSensor us{ us_trigger, us_echo };
  Motor left_motor{ left_motor_fwd, left_motor_bck, left_motor_en };
  Motor right_motor{ right_motor_fwd, right_motor_bck, right_motor_en };

  // Properly initialize the rover.
  rover = Rover{ us, left_motor, right_motor };
  rover.attach_servo(servo_pin);
  rover.reset();
}

void loop() {
  rover.run();
}
