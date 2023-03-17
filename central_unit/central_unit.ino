#include "display.h"
#include "joystick.h"

void setup() {
  Serial.begin(115200);
  display_setup();
  joystick_setup();
}

void loop() {
  display_loop();
  joystick_loop();
}
