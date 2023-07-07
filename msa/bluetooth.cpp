#include <Arduino.h>
#include <SoftwareSerial.h>

#define BT_TX_PIN 8
#define BT_RX_PIN 9

#define INTEGRATED_LED_PIN 25
#define EXTERNAL_LED_PIN 2

SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN);

//SerialPIO bt = SerialPIO(BT_TX_PIN, BT_RX_PIN);

uint8_t i, state, tmp;

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

  // if data on serial buffer, send them to hc05
  //while (Serial.available() > 0) {
  //  bt.print(Serial.read());
  //}

  // if data on softwareSerial buffer, show them on serial monitor
  i = 0;
  state = 0;
  while (bt.available() > 0) {
    tmp = bt.read();
    //if (i == 0) { //&& tmp != 10 && tmp != 13) {
      state = tmp;
      i++;
    //}
    digitalWrite(EXTERNAL_LED_PIN, HIGH);
    Serial.println("tmp: " + String(tmp));
    delay(500);
  }
  Serial.println("Read state: " + String(state));
  digitalWrite(EXTERNAL_LED_PIN, LOW);

  //if (state == 10) {
  //  Serial.println("LED 1: OFF");
  //  bt.println("LED 1: OFF");
  //  led_1 = false;
  //}

  Serial.print("state: ");
  Serial.println(state);
  bt.print("State: ");
  bt.println(state);

  digitalWrite(INTEGRATED_LED_PIN, LOW);
  delay(500);
}
