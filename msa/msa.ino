#include <Wire.h>
#include "display.h"
#include "bluetooth.h"
#include "controller.h"
#include "travel.h"
#include "imu.h"

void setup() {
  Serial.begin(115200);
  Wire.setSCL(6);
  Wire.setSDA(7);
  Wire.begin();
  display_setup();
  //bluetooth_setup();
  controller_setup();
  //travel_setup();
  //imu_setup();
}

void loop() {
  Serial.println(SCL);
  Serial.println(SDA);
  //bool recording = true;
  //imu_type imu = imu_loop(recording);
  uint8_t recording = controller_loop();
  //imu_loop(recording);
  //bool recording = true;
  bool error = false;
  //travel_type travel = travel_loop(recording);
  //travel_loop(recording);
  //if (!average_travel || !imu_loop(recording))
  //  error = true;
  display_loop(recording, error);
  //bluetooth_loop();
  //Serial.print("recording: ");
  //Serial.println(recording);
  delay(10);
}
