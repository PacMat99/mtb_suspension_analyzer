#include <Arduino.h>
#include <SoftwareSerial.h>
 
#define LED_PIN 14

#define BT_TX_PIN 13
#define BT_RX_PIN 12

// inizializzazione oggetto bluetooth
SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN); // rx_pin, tx_pin

char data = '1';

void bluetooth_setup() {
  bt.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void bluetooth_loop() {
  while (bt.available()) {
    delay(10);
    char data = bt.read();
    Serial.println("data: " + data);
    delay(1000);
  }
  Serial.println("data: " + data);
  if (data == '1') {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED: ON");
    //Serial.write("ON");
  }
  else if (data == '0') {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED: OFF");
    //Serial.write("OFF");
  }
  Serial.println("end bluetooth loop");
  delay(1000);
}
