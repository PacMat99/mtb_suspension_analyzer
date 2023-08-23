#include <string>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
//#include <Fonts/FreeMono9pt7b.h>
#include <Adafruit_LSM6DSOX.h>
#include "types.h"
#include "pins.h"

bool recording = false;
bool error = false;

int reading_n;
travel_type travel;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  controller_setup();
  imu_setup();
  display_setup();
  bluetooth_setup();
}

void loop() {
  Serial.print("SDA: "); Serial.println(SDA);
  Serial.print("SCL: "); Serial.println(SCL);
  
  controller_loop();
  travel_loop();
  imu_loop();
  display_loop();
  //bluetooth_loop();
  delay(10);
}

// -------------------- CONTROLLER --------------------
void controller_setup() {
  pinMode(BUZZER, OUTPUT);
  pinMode(CTRL_CONFIRM, INPUT_PULLUP);
}

bool controller_loop() {
  bool confirm = !digitalRead(CTRL_CONFIRM);
  Serial.print("confirm: "); Serial.println(confirm);
  if (confirm) {
    recording = !recording;
    if (recording) {
      reading_n = 0;
      travel.travel = 0;
      travel.average_travel = 0;
      travel.max_travel = 0;
      tone(BUZZER, 500, 500);
    }
    else
      tone(BUZZER, 200, 500);
    delay(500);
  }
  Serial.print("Recording: ");
  Serial.println(recording);
  return recording;
}

// -------------------- TRAVEL --------------------
void travel_loop() {
  if (recording) {
    analogReadResolution(12);

    int analog_value = analogRead(26);
    Serial.print("analog value: "); Serial.println(analog_value);

    travel.travel = map(analog_value, 0, 4096, 0, 150);
    Serial.print("travel: "); Serial.println(travel.travel);

    reading_n++;
    travel.average_travel = ((travel.average_travel * (reading_n - 1)) + travel.travel) / reading_n;

    if (travel.travel > travel.max_travel)
      travel.max_travel = travel.travel;

    Serial.print("Travel: "); Serial.println(travel.travel);
    Serial.print("Average travel: "); Serial.println(travel.average_travel);
    Serial.print("Max travel: "); Serial.println(travel.max_travel);
  }
}

// -------------------- IMU --------------------
Adafruit_LSM6DSOX sox_fork;
Adafruit_LSM6DSOX sox_frame;
imu_type imu;
imu_type imu_fork;
imu_type imu_frame;

/*
  Accel range [G]:
  +-2, +-4 (default), +-8, +-16

  Gyro range [degrees/s]:
  125, 250, 500, 1000, 2000 (default)

  Accel data rate [Hz]:
  0, 12.5, 26, 52, 104 (default), 208, 416, 833, 1660, 3330, 6660
  
  Gyro data rate [Hz]:
  0, 12.5, 26, 52, 104 (default), 208, 416, 833, 1660, 3330, 6660
*/
#define ACCEL_RANGE LSM6DS_ACCEL_RANGE_16_G
#define GYRO_RANGE LSM6DS_GYRO_RANGE_2000_DPS
#define ACCEL_DATA_RATE LSM6DS_RATE_104_HZ
#define GYRO_DATA_RATE LSM6DS_RATE_104_HZ

void imu_setup() {
  int i = 0;
  while (!sox_fork.begin_I2C() && i < 3) {
    Serial.println("LSM6DSOX fork error");
    i++;
    delay(500);
  }
  i = 0;
  while (!sox_frame.begin_I2C() && i < 3) {
    Serial.println("LSM6DSOX frame error");
    i++;
    delay(500);
  }
  if (i == 3)
    error = true;
  else {
    sox_fork.setAccelRange(ACCEL_RANGE);
    sox_fork.setGyroRange(GYRO_RANGE);
    sox_fork.setAccelDataRate(ACCEL_DATA_RATE);
    sox_fork.setGyroDataRate(GYRO_DATA_RATE);

    sox_frame.setAccelRange(ACCEL_RANGE);
    sox_frame.setGyroRange(GYRO_RANGE);
    sox_frame.setAccelDataRate(ACCEL_DATA_RATE);
    sox_frame.setGyroDataRate(GYRO_DATA_RATE);
  }
}

void imu_loop() {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  sox_fork.getEvent(&accel, &gyro, &temp);

  imu_fork.temperature = temp.temperature;
  Serial.print("\t\tTemperature "); Serial.print(imu_fork.temperature); Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  imu_fork.accel_x = accel.acceleration.x;
  imu_fork.accel_y = accel.acceleration.y;
  imu_fork.accel_z = accel.acceleration.z;
  Serial.print("\t\tFork accel X: "); Serial.print(imu_fork.accel_x);
  Serial.print(" \tY: "); Serial.print(imu_fork.accel_y);
  Serial.print(" \tZ: "); Serial.print(imu_fork.accel_z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  imu_fork.gyro_x = gyro.gyro.x;
  imu_fork.gyro_y = gyro.gyro.y;
  imu_fork.gyro_z = gyro.gyro.z;
  Serial.print("\t\tFork gyro X: "); Serial.print(imu_fork.gyro_x);
  Serial.print(" \tY: "); Serial.print(imu_fork.gyro_y);
  Serial.print(" \tZ: "); Serial.print(imu_fork.gyro_z);
  Serial.println(" radians/s \n");

  sox_frame.getEvent(&accel, &gyro, &temp);
  
  imu_frame.temperature = temp.temperature;
  Serial.print("\t\tTemperature "); Serial.print(imu_frame.temperature); Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  imu_frame.accel_x = accel.acceleration.x;
  imu_frame.accel_y = accel.acceleration.y;
  imu_frame.accel_z = accel.acceleration.z;
  Serial.print("\t\tFork accel X: "); Serial.print(imu_frame.accel_x);
  Serial.print(" \tY: "); Serial.print(imu_frame.accel_y);
  Serial.print(" \tZ: "); Serial.print(imu_frame.accel_z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  imu_frame.gyro_x = gyro.gyro.x;
  imu_frame.gyro_y = gyro.gyro.y;
  imu_frame.gyro_z = gyro.gyro.z;
  Serial.print("\t\tFork gyro X: "); Serial.print(imu_frame.gyro_x);
  Serial.print(" \tY: "); Serial.print(imu_frame.gyro_y);
  Serial.print(" \tZ: "); Serial.print(imu_frame.gyro_z);
  Serial.println(" radians/s \n");
}

// -------------------- DISPLAY --------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int timer = 0;
bool status = false;

void display_setup() {
  int i = 0;
  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS) && i < 3) {
    Serial.println("Failed to boot SSD1306");
    i++;
    delay(500);
  }
  if (i == 3)
    error = true;
  else {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 12);
    display.print("Booting...");
    display.display();
    delay(2000);
  }
}

void display_loop() {
  display.clearDisplay();
  display.setCursor(0, 12);
  if (error) {
    display.print("ERROR! :(");
    display.display();
  }
  else if (recording) {
    display.print("Recording");
    display.display();
  }
  else {
    // print max travel and average travel
    if (millis() >= timer) {
      timer = millis() + 2000;
      status = !status;
    }
    if (!status) {
      display.print("Ave: ");
      display.print(travel.average_travel);
    }
    if (status) {
      display.print("Max: ");
      display.print(travel.max_travel);
    }
    Serial.print("status: "); Serial.println(status);

    display.display();
  }
}

// -------------------- BLUETOOTH --------------------
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
}
