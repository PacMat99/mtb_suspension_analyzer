#include "display.h"
#include "controller.h"
#include "tof.h"
#include "imu.h"

void setup() {
  Serial.begin(115200);
  display_setup();
  controller_setup();
  tof_setup();
  imu_setup();
}

void loop() {
  display_loop();
  controller_loop();
  tof_loop();
  imu_loop();
  delay(100);
}

/* ESP8266 PINOUT
#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

static const uint8_t LED_BUILTIN = 16;
static const uint8_t BUILTIN_LED = 16;

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10 = 1;
*/