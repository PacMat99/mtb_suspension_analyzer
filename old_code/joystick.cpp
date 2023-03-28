#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define BUTTON_PIN 5

void joystick_setup(void) {
  if (!ads.begin()) {
    delay(5000);
    Serial.println("Failed to initialize ADS.");
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void joystick_loop(void) {
  int16_t x_pin, y_pin, button_pin;

  x_pin = ads.readADC_SingleEnded(0);
  y_pin = ads.readADC_SingleEnded(1);
  button_pin = !digitalRead(BUTTON_PIN);

  Serial.println("-----------------------------------------------------------");
  Serial.print("x_pin: "); Serial.println(x_pin);
  Serial.print("y_pin: "); Serial.println(y_pin);
  Serial.print("button_pin: "); Serial.println(button_pin);
}
