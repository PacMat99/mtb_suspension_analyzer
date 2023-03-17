#include "display.h"

void setup() {
  Serial.begin(9600);
  display_setup();
}

void loop() {
  display_loop();
}
