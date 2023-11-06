// The central file for assigning pins and configuring the robot's behavior.
// TODO: Pick a remote rescue button.
#ifndef ESP_CONFIG_H
#define ESP_CONFIG_H

// HC-SR04 Ultrasonic Sensor
// -------------------------
const byte us_echo    = A0; // Echo (digital)
const byte us_trigger = A1; // Trigger (digital)

// H-Bridge for DC Motor (left)
// ----------------------------
const byte left_motor_en  = 5; // Enable (PWM using OC0B, timer 0)
const byte left_motor_bck = 2; // Input 1
const byte left_motor_fwd = 4; // Input 2

// H-Bridge for DC Motor (right)
// -----------------------------
const byte right_motor_en  = 6; // Enable (PWM using OC0A, timer 0)
const byte right_motor_bck = 7; // Input 1
const byte right_motor_fwd = 8; // Input 2

// 1838 IR Remote Receiver
// -----------------------
const byte ir_recv_signal = 12; // Signal pin
const byte ir_recv_rescue = 0;  // "Rescue" button code

// Servo Motor
// -----------
const byte servo_pin = A2;

#endif
