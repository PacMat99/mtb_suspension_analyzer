#include <Arduino.h>
#include "mcp7940.h"

void setup() {
	Serial.begin(9600);
}

void loop() {
	setTime();
	delay(1000);
}
