#include <ESP8266WiFi.h>
#include "tof.h"

void setup() {
  Serial.begin(115200);
  tof_setup();
  Serial.println(WiFi.macAddress());
}

void loop() {
  //tof_loop();
  delay(100);
}
