// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void initialize_ds3231() {
	if (!rtc.begin()) {
	Serial.println("Failed to find RTC");
	Serial.flush();
	while (1)
		delay(10);
	}
	Serial.println("RTC Found!");

	if (rtc.lostPower()) {
		Serial.println("RTC lost power, let's set the time!");
		// When time needs to be set on a new device, or after a power loss, the
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}

	// When time needs to be re-set on a previously configured device, the
	// following line sets the RTC to the date & time this sketch was compiled
	rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	// This line sets the RTC with an explicit date & time, for example to set
	// January 21, 2014 at 3am you would call:
	// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

void read_ds3231() {
	DateTime now = rtc.now();

	char datetime[] = "hh:mm:ss-DD/MM/YYYY";
	now.toString(datetime);
	Serial.println(datetime);

	Serial.print("Temperature: ");
	Serial.print(rtc.getTemperature());
	Serial.println(" C");

	Serial.println();
}
