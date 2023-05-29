#include "display.h"
#include "bluetooth.h"
#include "controller.h"
#include "tof.h"
#include "imu.h"

void setup() {
  Serial.begin(115200);
  display_setup();
  bluetooth_setup();
  //controller_setup();
  tof_setup();
  imu_setup();
}

void loop() {
  //bool recording = controller_loop();
  bool recording = true;
  bool error = false;
  if (!tof_loop() || !imu_loop())
    error = true;
  display_loop(recording, error);
  bluetooth_loop();
}
