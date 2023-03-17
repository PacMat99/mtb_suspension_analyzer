#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

void joystick_setup(void) {
  if (!ads.begin()) {
    delay(5000);
    Serial.println("Failed to initialize ADS.");
  }
}

void joystick_loop(void) {
  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

  Serial.println("-----------------------------------------------------------");
  Serial.print("AIN0: "); Serial.print(adc0);// Serial.print("  "); Serial.print(volts0); Serial.println("V");
  Serial.print("AIN1: "); Serial.print(adc1);// Serial.print("  "); Serial.print(volts1); Serial.println("V");
  Serial.print("AIN2: "); Serial.print(adc2);// Serial.print("  "); Serial.print(volts2); Serial.println("V");
  Serial.print("AIN3: "); Serial.print(adc3);// Serial.print("  "); Serial.print(volts3); Serial.println("V");
}
