#include <sys/_stdint.h>
#include <Arduino.h>

#define CTRL_CONFIRM 18
#define BUZZER 19

uint8_t recording = false;

void controller_setup(void) {
  pinMode(BUZZER, OUTPUT);
  pinMode(CTRL_CONFIRM, INPUT_PULLUP);
}

uint8_t controller_loop(void) {
  uint8_t confirm = !digitalRead(CTRL_CONFIRM);
  Serial.print("confirm: "); Serial.println(confirm);
  if (confirm) {
    recording = !recording;
    if (recording)
      tone(BUZZER, 500, 500);
    else
      tone(BUZZER, 200, 500);
    delay(500);
  }
  Serial.print("Recording: ");
  Serial.println(recording);
  return recording;
}
