#include <Wire.h> // i2c communication library
#include <string.h>

#define LEN 4

typedef struct  { 
	uint8_t Second; 
	uint8_t Minute; 
	uint8_t Hour; 
	uint8_t Wday;   // day of week, sunday is day 1
	uint8_t Day;
	uint8_t Month; 
	uint8_t Year;   // offset from 1970; 
} tmElements_t;

//struttura timeelements
tmElements_t tm;

// decimal to BCD conversion
byte dec2bcd(byte val) {
	return ( ((val / 10) * 16) + (val % 10));
}

// BCD to decimal conversion
byte bcd2dec(byte num) {
	return (((num / 16) * 10) + (num % 16));
}

int equalTo(char *tmp, const char *month) {
	int i, check;

	i = 0;
	check = 1;
	while (i < LEN && check) {
		if (tmp[i] != month[i])
			check--;
		i++;
	}

	return check;
}

void setTime() {
	int i, day, month, year;
	char tmp[4];
	const char months[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	day = ((__DATE__[4] - 48) * 10) + (__DATE__[5] - 48);

	month = 0;
	for (i = 0; i < LEN-1; i++) {
		tmp[i] = __DATE__[i];
	}
	tmp[i] = '\0';
	while (!equalTo(tmp, months[month]) && month < 12) {
		month++;
	}
	month++;

	year = ((__DATE__[7] - 48) * 1000) + ((__DATE__[8] - 48) * 100) + ((__DATE__[9] - 48) * 10) + (__DATE__[10] - 48);
	Serial.println("Date: " + String(day) + "/" + String(month) + "/" + String(year));

	int h, m, s;
	h = ((__TIME__[0] - 48) * 10) + (__TIME__[1] - 48);
	m = ((__TIME__[3] - 48) * 10) + (__TIME__[4] - 48);
	s = ((__TIME__[6] - 48) * 10) + (__TIME__[7] - 48);
	Serial.println("Time: " + String(h) + ":" + String(m) + ":" + String(s));


	// begin MCP7940N communication on RTC address 0x6F
	////Wire.beginTransmission(0x6F);
	////Wire.write(0x00);
	// specify time and date and enable ST (oscillator enabled)
	////Wire.write(dec2bcd(0) | 0x80); // seconds and ST to high logic level
	// previous line: | 0x80 -> bit a bit logic OR necessary to set the first bit to 1 to enable ST without changing inserted seconds
	////Wire.write(dec2bcd(23)); // minutes
	////Wire.write(dec2bcd(10));  // hours
	////Wire.write(dec2bcd(2) | 0x08);  // day of the week and VBAT on
	////Wire.write(dec2bcd(8)); // day
	////Wire.write(dec2bcd(2)); // month
	////Wire.write(dec2bcd(22)); // year
	////Wire.endTransmission();
}

tmElements_t getTime() {
	// START WITH RTC SETUP CODE
	// begin MCP7940N communication on RTC address 0x6F
	Wire.beginTransmission(0x6F);
	Wire.write(0x00);
	Wire.endTransmission();

	//richiedo 7 byte dal dispositivo con
	//indirizzo 0x6F
	Wire.requestFrom(0x6F, 7);
	//recupero i 7 byte relativi ai
	//corrispondenti registri
	tm.Second = bcd2dec(Wire.read() & 0x7f); // seconds 
	tm.Minute = bcd2dec(Wire.read()); // minutes
	tm.Hour = bcd2dec(Wire.read() & 0x3f); // hours
	tm.Wday = bcd2dec(Wire.read() & 0x07); // day of the week
	tm.Day = bcd2dec(Wire.read()); // day
	tm.Month = bcd2dec(Wire.read() & 0x1F ); // month 
	tm.Year = bcd2dec(Wire.read()); // year

	return tm;
}

void printDate(tmElements_t tm) {
	Serial.print("Orario corrente: ");
	Serial.print(tm.Hour);
	Serial.print(":");
	Serial.print(tm.Minute );
	Serial.print(":");
	Serial.println(tm.Second);

	Serial.print("Giorno della settimana: ");
	Serial.println(tm.Wday);

	Serial.print("Data corrente: ");
	Serial.print(tm.Day);
	Serial.print("/");
	Serial.print(tm.Month);
	Serial.print("/");
	Serial.println(tm.Year);
}
