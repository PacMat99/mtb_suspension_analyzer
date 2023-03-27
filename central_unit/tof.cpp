#include <Arduino.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X tof = Adafruit_VL53L0X();

void tof_setup(void) {
  if (!tof.begin()) {
    Serial.println("Failed to boot VL53L0X");
    delay(5000);
  }
}

void tof_loop(void) {
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  tof.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {
    // phase failures have incorrect data
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  }
  else {
    Serial.println(" out of range ");
  }
}
