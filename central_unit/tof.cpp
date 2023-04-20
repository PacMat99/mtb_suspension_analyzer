#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void tof_setup() {
  Wire.begin();

  sensor.setTimeout(500);
  while (!sensor.init()) {
    Serial.println("Failed to detect and initialize sensor!");
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);
}

void tof_loop() {
  int travel = sensor.readRangeContinuousMillimeters();
  //Serial.println(travel);
  if (sensor.timeoutOccurred()) {
    Serial.println(" TIMEOUT");
  }
}
