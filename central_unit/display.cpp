#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono12pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Address 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void display_setup(void) {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    delay(5000);
    Serial.println("SSD1306 allocation failed");
  }

  display.setFont(&FreeMono12pt7b);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.println("Welcome!");
  display.display();
  delay(100);
}

void display_loop(void) {
  display.startscrollright(0x00, 0x0F);
 
  delay(2000);
  
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);

  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
 
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);

  delay(2000);
  display.stopscroll();
  delay(1000);
}
