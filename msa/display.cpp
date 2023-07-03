#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Fonts/FreeMono9pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Address 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void display_setup(void) {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed to boot SSD1306");
    delay(1000);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.print("Booting...");
  display.display();
  delay(2000);
}

void display_loop(bool recording, bool error) {
  display.clearDisplay();
  display.setCursor(0, 12);
  if (error)
    display.print("ERROR! :(");
  else if (recording)
    display.print("Recording");
  else
    display.print("Waiting");
  display.display();
}
