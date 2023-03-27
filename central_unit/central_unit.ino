#include "wait.h"
#include "display.h"
#include "controller.h"
#include "tof.h"

void setup() {
  Serial.begin(115200);
  display_setup();
  controller_setup();
  tof_setup();
}

void loop() {
  display_loop();
  controller_loop();
  tof_loop();
  delay(100);
  //wait(1000);
}
