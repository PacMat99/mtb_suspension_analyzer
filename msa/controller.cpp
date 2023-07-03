#include <Arduino.h>

#define CTRL_CONFIRM 20
#define BUZZER 27

bool recording = false;

void controller_setup(void) {
  pinMode(BUZZER, OUTPUT);
  pinMode(CTRL_CONFIRM, INPUT_PULLUP);
}

bool controller_loop(void) {
  int confirm = !digitalRead(CTRL_CONFIRM);
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
