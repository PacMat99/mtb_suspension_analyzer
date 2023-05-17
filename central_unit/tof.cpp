#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

void tof_setup() {
  Wire.begin();

  sensor.setTimeout(0);
  if (!sensor.init()) {
    Serial.println("TOF error");
    delay(1000);
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);
}

bool tof_loop() {
  int travel = sensor.readRangeContinuousMillimeters();
  Serial.print("Fork travel: ");
  Serial.println(travel);
  if (sensor.timeoutOccurred() || travel >= 8190) {
    Serial.println("TOF error");
    return false;
  }

  return true;
}
