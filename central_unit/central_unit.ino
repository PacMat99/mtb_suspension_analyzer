#include <Arduino.h>
#include "display.h"

void setup() {
	Serial.begin(9600);
	setupDisplay();
}

void loop() {
	delay(1000);
}
