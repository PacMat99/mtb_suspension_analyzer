#include <Arduino.h>
#include <VL53L0X.h>
#include "travel.h"

VL53L0X sensor;

int n = 0;
int tot = 0;

void travel_setup() {
  sensor.setTimeout(0);
  while (!sensor.init()) {
    Serial.println("travel error");
    delay(500);
  }

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);
}

uint16_t travel_loop(bool recording) {
  analogReadResolution(12);
  uint16_t travel = analogRead(26);
  Serial.print("analog value: ");
  Serial.println(travel);
  travel = map(travel, 0, 4096, 0, 150);
  Serial.print("mapped value: ");
  Serial.println(travel);
  //if (!recording) {
  //  n = 0;
  //  tot = 0;
  //}
  //int travel = sensor.readRangeContinuousMillimeters();
  //Serial.print("Fork travel: ");
  //Serial.println(travel);
  //Serial.print("SDA: ");
  //Serial.println(SDA);
  //Serial.print("SCL: ");
  //Serial.println(SCL);
  //if (sensor.timeoutOccurred() || travel >= 8190) {
  //  Serial.println("travel error");
  //  return 0;
  //}

  return travel;
}










//#include <Arduino.h>
//#include <Wire.h>
//#include <VL53L0X.h>
//#include "travel.h"
//
//VL53L0X sensor;
//
////travel_type travel;
//
//void travel_setup() {
//  //Wire.begin();
//  //sensor.setTimeout(500);
//  //while (!sensor.init()) {
//  //  Serial.println("travel error");
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
//travel_type travel_loop(bool recording) {
//  travel.error = false;
//
//  if (!recording) {
//    travel.reading_n = 0;
//    travel.max_travel = 0;
//  }
//  travel.travel = sensor.readRangeContinuousMillimeters();
//
//  travel.reading_n++;
//  travel.average_travel = ((travel.average_travel * (travel.reading_n - 1)) + travel.travel) / travel.reading_n;
//
//  if (travel.travel > travel.max_travel)
//    travel.max_travel = travel.travel;
//
//  Serial.print("Travel: ");
//  Serial.println(travel.travel);
//  Serial.print("Average travel: ");
//  Serial.println(travel.average_travel);
//  Serial.print("Max travel: ");
//  Serial.println(travel.max_travel);
//
//  Serial.print("SDA: ");
//  Serial.println(SDA);
//  Serial.print("SCL: ");
//  Serial.println(SCL);
//
//  if (sensor.timeoutOccurred() || travel.travel >= 8190) {
//    travel.error = true;
//  }
//
//  Serial.print(sensor.readRangeContinuousMillimeters());
//  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
//
//  Serial.println();
//
//  return travel;
//}
//
//void travel_loop(bool recording) {
//  Serial.print(sensor.readRangeContinuousMillimeters());
//  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
//
//  Serial.println();
//}
