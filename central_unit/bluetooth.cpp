#include <Arduino.h>
#include <SoftwareSerial.h>

#define BT_TX_PIN 12
#define BT_RX_PIN 13

#define LED_PIN_1 5
#define LED_PIN_2 6

SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

int state = 0;

bool led_1 = false;
bool led_2 = false;

void bluetooth_setup() {
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  bt.begin(9600);
}

void bluetooth_loop() {
  if (bt.available() > 0)
    state = bt.read();
  
  if (state == 10) {
    Serial.println("LED 1: OFF");
    bt.println("LED 1: OFF");
    led_1 = false;
  }
  else if (state == 15) {
    Serial.println("LED 1: ON");
    bt.println("LED 1: ON");
    led_1 = true;
  }

  if (state == 20) {
    Serial.println("LED 2: OFF");
    bt.println("LED 2: OFF");
    led_2 = false;
  }
  else if (state == 25) {
    Serial.println("LED 2: ON");
    bt.println("LED 2: ON");
    led_2 = true;
  }

  if (led_1)
    digitalWrite(LED_PIN_1, HIGH);
  else
    digitalWrite(LED_PIN_1, LOW);

  if (led_2)
    digitalWrite(LED_PIN_2, HIGH);
  else
    digitalWrite(LED_PIN_2, LOW);

  Serial.println("state: " + String(state));
  delay(500);
}
