#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "tof.h"

VL53L0X sensor;

int n = 0;
int tot = 0;

void tof_setup() {
  Wire.begin();

  sensor.setTimeout(0);
  while (!sensor.init()) {
    Serial.println("TOF error");
    delay(500);
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);
}

float tof_loop(bool recording) {
  if (!recording) {
    n = 0;
    tot = 0;
  }
  int travel = sensor.readRangeContinuousMillimeters();
  Serial.print("Fork travel: ");
  Serial.println(travel);
  Serial.print("SDA: ");
  Serial.println(SDA);
  Serial.print("SCL: ");
  Serial.println(SCL);
  if (sensor.timeoutOccurred() || travel >= 8190) {
    Serial.println("TOF error");
    return 0;
  }

  return travel;
}










//#include <Arduino.h>
//#include <Wire.h>
//#include <VL53L0X.h>
//#include "tof.h"
//
//VL53L0X sensor;
//
////tof_type tof;
//
//void tof_setup() {
//  //Wire.begin();
//  //sensor.setTimeout(500);
//  //while (!sensor.init()) {
//  //  Serial.println("TOF error");
//  //}
//
//  Wire.begin();
//
//  sensor.setTimeout(500);
//  if (!sensor.init())
//  {
//    Serial.println("Failed to detect and initialize sensor!");
//    while (1) {}
//  }
//
//  //bool setMeasurementTimingBudget(200000);
//
//  // Start continuous back-to-back mode (take readings as
//  // fast as possible).  To use continuous timed mode
//  // instead, provide a desired inter-measurement period in
//  // ms (e.g. sensor.startContinuous(100)).
//  //sensor.startContinuous(100);
//  sensor.startContinuous();
//}
//
//tof_type tof_loop(bool recording) {
//  tof.error = false;
//
//  if (!recording) {
//    tof.reading_n = 0;
//    tof.max_travel = 0;
//  }
//  tof.travel = sensor.readRangeContinuousMillimeters();
//
//  tof.reading_n++;
//  tof.average_travel = ((tof.average_travel * (tof.reading_n - 1)) + tof.travel) / tof.reading_n;
//
//  if (tof.travel > tof.max_travel)
//    tof.max_travel = tof.travel;
//
//  Serial.print("Travel: ");
//  Serial.println(tof.travel);
//  Serial.print("Average travel: ");
//  Serial.println(tof.average_travel);
//  Serial.print("Max travel: ");
//  Serial.println(tof.max_travel);
//
//  Serial.print("SDA: ");
//  Serial.println(SDA);
//  Serial.print("SCL: ");
//  Serial.println(SCL);
//
//  if (sensor.timeoutOccurred() || tof.travel >= 8190) {
//    tof.error = true;
//  }
//
//  Serial.print(sensor.readRangeContinuousMillimeters());
//  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
//
//  Serial.println();
//
//  return tof;
//}
//
//void tof_loop(bool recording) {
//  Serial.print(sensor.readRangeContinuousMillimeters());
//  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
//
//  Serial.println();
//}
