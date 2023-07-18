#include <Wire.h>
#include "display.h"
#include "bluetooth.h"
#include "controller.h"
#include "tof.h"
#include "imu.h"

void setup() {
  Serial.begin(115200);
  Wire.begin();
  //display_setup();
  //bluetooth_setup();
  //controller_setup();
  tof_setup();
  //imu_setup();
}

void loop() {
  bool recording = true;
  //imu_type imu = imu_loop(recording);
  //imu_loop(recording);
  //uint8_t recording = controller_loop();
  //bool recording = true;
  bool error = false;
  //tof_type tof = tof_loop(recording);
  tof_loop(recording);
  //if (!average_travel || !imu_loop(recording))
  //  error = true;
  //display_loop(recording, error);
  //bluetooth_loop();
  //Serial.print("recording: ");
  //Serial.println(recording);
}
