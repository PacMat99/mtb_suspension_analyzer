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

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  controller_setup();
  //imu_setup();
  display_setup();
  bluetooth_setup();
}

void loop() {
  Serial.print("SDA: "); Serial.println(SDA);
  Serial.print("SCL: "); Serial.println(SCL);
  
  controller_loop();
  travel_loop();
  //imu_loop();
  //display_loop();
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

// -------------------- TRAVEL --------------------
int reading_n = 0;
travel_type travel;

void travel_loop() {
  if (!recording) {
    reading_n = 0;
    travel.travel = 0;
    travel.average_travel = 0;
    travel.max_travel = 0;
  }
  else {
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
Adafruit_LSM6DSOX sox;
imu_type imu;
imu_type imu_fork;
imu_type imu_frame;

void imu_setup() {
  while (!sox.begin_I2C()) {
    Serial.println("LSM6DSOX error");
    delay(500);
  }

  // sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (sox.getAccelRange()) {
    case LSM6DS_ACCEL_RANGE_2_G:
      strcpy(imu.accel_range, "+-2G");
      break;
    case LSM6DS_ACCEL_RANGE_4_G:
      strcpy(imu.accel_range, "+-4G");
      break;
    case LSM6DS_ACCEL_RANGE_8_G:
      strcpy(imu.accel_range, "+-8G");
      break;
    case LSM6DS_ACCEL_RANGE_16_G:
      strcpy(imu.accel_range, "+-16G");
      break;
  }
  Serial.println(imu.accel_range);

  // sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  Serial.print("Gyro range set to: ");
  switch (sox.getGyroRange()) {
    case LSM6DS_GYRO_RANGE_125_DPS:
      strcpy(imu.gyro_range, "125 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_250_DPS:
      strcpy(imu.gyro_range, "250 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_500_DPS:
      strcpy(imu.gyro_range, "500 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_1000_DPS:
      strcpy(imu.gyro_range, "1000 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_2000_DPS:
      strcpy(imu.gyro_range, "2000 degrees/s");
      break;
  }
  Serial.println(imu.gyro_range);

  // sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Accelerometer data rate set to: ");
  switch (sox.getAccelDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      strcpy(imu.accel_data_rate, "0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      strcpy(imu.accel_data_rate, "12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      strcpy(imu.accel_data_rate, "26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      strcpy(imu.accel_data_rate, "52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      strcpy(imu.accel_data_rate, "104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      strcpy(imu.accel_data_rate, "208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      strcpy(imu.accel_data_rate, "416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      strcpy(imu.accel_data_rate, "833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      strcpy(imu.accel_data_rate, "1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      strcpy(imu.accel_data_rate, "3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      strcpy(imu.accel_data_rate, "6.66 KHz");
      break;
  }
  Serial.println(imu.accel_data_rate);

  // sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  Serial.print("Gyro data rate set to: ");
  switch (sox.getGyroDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      strcpy(imu.gyro_data_rate, "0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      strcpy(imu.gyro_data_rate, "12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      strcpy(imu.gyro_data_rate, "26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      strcpy(imu.gyro_data_rate, "52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      strcpy(imu.gyro_data_rate, "104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      strcpy(imu.gyro_data_rate, "208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      strcpy(imu.gyro_data_rate, "416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      strcpy(imu.gyro_data_rate, "833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      strcpy(imu.gyro_data_rate, "1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      strcpy(imu.gyro_data_rate, "3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      strcpy(imu.gyro_data_rate, "6.66 KHz");
      break;
  }
  Serial.println(imu.gyro_data_rate);
}

imu_type imu_loop() {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  imu.temperature = temp.temperature;

  Serial.print("\t\tTemperature ");
  Serial.print(imu.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  imu.accel_x = accel.acceleration.x;
  imu.accel_y = accel.acceleration.y;
  imu.accel_z = accel.acceleration.z;
  Serial.print("\t\tAccel X: ");
  Serial.print(imu.accel_x);
  Serial.print(" \tY: ");
  Serial.print(imu.accel_y);
  Serial.print(" \tZ: ");
  Serial.print(imu.accel_z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  imu.gyro_x = gyro.gyro.x;
  imu.gyro_y = gyro.gyro.y;
  imu.gyro_z = gyro.gyro.z;
  Serial.print("\t\tGyro X: ");
  Serial.print(imu.gyro_x);
  Serial.print(" \tY: ");
  Serial.print(imu.gyro_y);
  Serial.print(" \tZ: ");
  Serial.print(imu.gyro_z);
  Serial.println(" radians/s ");
  Serial.println();

  imu.error = false;

  return imu;
}

// -------------------- DISPLAY --------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int start;
int end;
int i = 0;

void display_setup() {
  while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Failed to boot SSD1306");
    delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.print("Booting...");
  display.display();
  Serial.println("display setup finished");
  delay(2000);
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
    start = millis();
    end = start + 2000;
    if (i++ == 0) {
      display.print("Ave: ");
      display.print(travel.average_travel);
    }
    if (i-- == 1) {
      display.print("Max: ");
      display.print(travel.max_travel);
    }
    Serial.print("i: "); Serial.println(i);

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
