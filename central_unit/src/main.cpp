#include <Arduino.h>
#include "mpu6050.h"
#include "ds3231.h"

void setup() {
	Serial.begin(115200);
	while (!Serial)
		delay(10);

	//initialize_mpu6050();
	initialize_ds3231();
}

void loop() {
	read_mpu6050();
	read_ds3231();
	delay(1000);
}
