#include "display.h"

void setup() {
  Serial.begin(115200);
  display_setup();
}

void loop() {
  display_loop();
}
