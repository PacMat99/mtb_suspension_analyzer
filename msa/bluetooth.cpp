#include <sys/_stdint.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#define BT_TX_PIN 0
#define BT_RX_PIN 1

#define INTEGRATED_LED_PIN 25
#define EXTERNAL_LED_PIN 21

SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

//SerialPIO bt = SerialPIO(BT_TX_PIN, BT_RX_PIN);

int state = 0;
int tmp;

void bluetooth_setup() {
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
  pinMode(INTEGRATED_LED_PIN, OUTPUT);
  pinMode(EXTERNAL_LED_PIN, OUTPUT);
  digitalWrite(INTEGRATED_LED_PIN, LOW);
  digitalWrite(EXTERNAL_LED_PIN, LOW);
  bt.begin(9600);
}

void bluetooth_loop() {
  digitalWrite(INTEGRATED_LED_PIN, HIGH);

  // if data on softwareSerial buffer, show them on serial monitor
  while (bt.available() > 0) {
    tmp = bt.read();
    if (tmp != 10 && tmp != 13 && tmp != 255) {
      state = tmp;
      delay(2000);
    }

    Serial.println("tmp: " + String(tmp));
  }

  if (state == 2) {
    digitalWrite(EXTERNAL_LED_PIN, HIGH);
    bt.println("EXTERNAL LED: ON");
  }
  else {
    digitalWrite(EXTERNAL_LED_PIN, LOW);
    bt.println("EXTERNAL LED: OFF");
  }

  Serial.print("state: ");
  Serial.println(state);
  bt.print("state: ");
  bt.println(state);

  digitalWrite(INTEGRATED_LED_PIN, LOW);
  delay(500);
}
