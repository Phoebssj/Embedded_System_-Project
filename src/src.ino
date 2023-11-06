#include <HardwareSerial.h>
#include "config.h"

// Explicitly declare `Serial` to get nice editor suggestions.
extern HardwareSerial Serial;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello, Arduino!");
  delay(1000);
}
