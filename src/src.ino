#include <Arduino.h>
#include <HardwareSerial.h>
#include "config.h"
#include "UltrasonicSensor.h"

// Explicitly declare `Serial` to get nice editor suggestions.
extern HardwareSerial Serial;

UltrasonicSensor us{ us_trigger, us_echo };

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
}
