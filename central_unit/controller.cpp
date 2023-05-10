#include <Arduino.h>

#define BUZZER 10
//#define CTRL_UP 13
#define CTRL_CONFIRM 12
//#define CTRL_DOWN 14

bool recording = false;

void controller_setup(void) {
  //pinMode()
  //pinMode(CTRL_UP, INPUT);
  pinMode(CTRL_CONFIRM, INPUT);
  //pinMode(CTRL_DOWN, INPUT);
}

bool controller_loop(void) {
  //int up = digitalRead(CTRL_UP);
  int confirm = digitalRead(CTRL_CONFIRM);
  //int down = digitalRead(CTRL_DOWN);
  //Serial.print("up: "); Serial.println(up);
  Serial.print("confirm: "); Serial.println(confirm);
  //Serial.print("down: "); Serial.println(down);
  if (confirm) {//up || confirm || down) {
    recording = !recording;
    if (recording)
      tone(BUZZER, 500, 100);
    delay(500);
  }
  return recording;
}
