#include <stdio.h>
#include <string>

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include <Wire.h>
#include <SPI.h>
#include <lib/Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include <SoftwareSerial.h>
//#include <Fonts/FreeMono9pt7b.h>
#include <Adafruit_LSM6DSOX.h>

// -------- CONTROLLER --------
#define CTRL_CONFIRM 18
#define BUZZER 19

uint8_t controller_loop(uint8_t recording) {
    uint8_t confirm = gpio_get(CTRL_CONFIRM);
    printf("confirm: %d", confirm);
    if (confirm) {
        recording = !recording;
        if (recording) {
            tone(BUZZER, 500, 500);
        }
        else
            tone(BUZZER, 200, 500);
        sleep_ms(500);
    }
    printf("Recording: %d", recording);
    return recording;
}

// -------- IMU CODE --------
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

struct imu_type {
    float temperature;
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
};
typedef struct imu_type timu_type;

Adafruit_LSM6DSOX sox_fork;
Adafruit_LSM6DSOX sox_frame;
imu_type imu_fork;
imu_type imu_frame;

void imu_loop() {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    sox_fork.getEvent(&accel, &gyro, &temp);

    imu_fork.temperature = temp.temperature;
    printf("\t\tTemperature %.3f deg C\n", imu_fork.temperature);

    /* Display the results (acceleration is measured in m/s^2) */
    imu_fork.accel_x = accel.acceleration.x;
    imu_fork.accel_y = accel.acceleration.y;
    imu_fork.accel_z = accel.acceleration.z;
    printf("\t\tFork accel X: %.3f\tY: %.3f\tZ: %.3fm/s^2\n", imu_fork.accel_x, imu_fork.accel_y, imu_fork.accel_z);

    /* Display the results (rotation is measured in rad/s) */
    imu_fork.gyro_x = gyro.gyro.x;
    imu_fork.gyro_y = gyro.gyro.y;
    imu_fork.gyro_z = gyro.gyro.z;
    printf("\t\tFork gyro X: %.3f\tY: %.3f\tZ: %.3fradians/s\n", imu_fork.gyro_x, imu_fork.gyro_y, imu_fork.gyro_z);

    sox_frame.getEvent(&accel, &gyro, &temp);

    imu_frame.temperature = temp.temperature;
    printf("\t\tTemperature %.3f deg C\n", imu_frame.temperature);

    /* Display the results (acceleration is measured in m/s^2) */
    imu_frame.accel_x = accel.acceleration.x;
    imu_frame.accel_y = accel.acceleration.y;
    imu_frame.accel_z = accel.acceleration.z;
    printf("\t\tFork accel X: %.3f\tY: %.3f\tZ: %.3fm/s^2\n", imu_frame.accel_x, imu_frame.accel_y, imu_frame.accel_z);

    /* Display the results (rotation is measured in rad/s) */
    imu_frame.gyro_x = gyro.gyro.x;
    imu_frame.gyro_y = gyro.gyro.y;
    imu_frame.gyro_z = gyro.gyro.z;
    printf("\t\tFork gyro X: %.3f\tY: %.3f\tZ: %.3fradians/s\n", imu_frame.gyro_x, imu_frame.gyro_y, imu_frame.gyro_z);
}

// -------- DISPLAY CODE --------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Address 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
uint8_t timer = 0;
uint8_t change_info = 0;

void display_loop(uint8_t recording, uint8_t error) {
    display.clearDisplay();
    display.setCursor(0, 12);
    if (error) {
        display.print("ERROR! :(");
        display.display();
    }
    else if (recording) {
        char str[] = "recording";
        uint8_t len = strlen(str);
        while (strlen(str) > 3) {
            str[len - 1] = '\0';
            display.clearDisplay();
            display.print(str);
            display.display();
            sleep_ms(500);
        }
    }
    else {
        // print max travel and average travel
        if (millis() >= timer) {
            timer = millis() + 2000;
            change_info = !change_info;
        }
        if (!change_info) {
            display.print("Ave: ");
            display.print(travel.average_travel);
        }
        if (change_info) {
            display.print("Max: ");
            display.print(travel.max_travel);
        }
        printf("change_info: %d\n", change_info);

        display.display();
    }
}

// -------- BLUETOOTH CODE --------
#define BT_TX_PIN 0
#define BT_RX_PIN 1
#define INTEGRATED_LED_PIN 25
#define EXTERNAL_LED_PIN 21
SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
//SerialPIO bt = SerialPIO(BT_TX_PIN, BT_RX_PIN);

void bluetooth_loop() {
    int state = 0;
    int tmp;

    digitalWrite(INTEGRATED_LED_PIN, HIGH);

    // if data on softwareSerial buffer, show them on serial monitor
    while (bt.available() > 0) {
        tmp = bt.read();
        if (tmp != 10 && tmp != 13 && tmp != 255) {
            state = tmp;
        }

        printf("tmp: %s\n", tmp);
    }

    if (state == 2) {
        digitalWrite(EXTERNAL_LED_PIN, HIGH);
        bt.println("EXTERNAL LED: ON");
    }
    else {
        digitalWrite(EXTERNAL_LED_PIN, LOW);
        bt.println("EXTERNAL LED: OFF");
    }

    printf("state: %d\n", state);
    bt.print("state: "); bt.println(state);

    digitalWrite(INTEGRATED_LED_PIN, LOW);
}

// -------- MAIN --------
int main() {
    // START CONTROLLER SETUP
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
    gpio_init(CTRL_CONFIRM);
    gpio_set_dir(CTRL_CONFIRM, GPIO_IN);
    // END CONTROLLER SETUP

    // START DISPLAY SETUP
    while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("Failed to boot SSD1306");
        sleep_ms(500);
    }
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 12);
    display.print("Booting...");
    display.display();
    sleep_ms(2000);
    // END DISPLAY SETUP

    // START IMU SETUP
    while (!sox_fork.begin_I2C(0x6A)) {
        display_loop(0, 1);
        sleep_ms(500);
    }
    while (!sox_frame.begin_I2C(0x6B)) {
        display_loop(0, 1);
        sleep_ms(500);
    }

    sox_fork.setAccelRange(ACCEL_RANGE);
    sox_fork.setGyroRange(GYRO_RANGE);
    sox_fork.setAccelDataRate(ACCEL_DATA_RATE);
    sox_fork.setGyroDataRate(GYRO_DATA_RATE);

    sox_frame.setAccelRange(ACCEL_RANGE);
    sox_frame.setGyroRange(GYRO_RANGE);
    sox_frame.setAccelDataRate(ACCEL_DATA_RATE);
    sox_frame.setGyroDataRate(GYRO_DATA_RATE);
    // END IMU SETUP

    // START BLUETOOTH SETUP
    pinMode(BT_RX_PIN, INPUT);
    pinMode(BT_TX_PIN, OUTPUT);
    pinMode(INTEGRATED_LED_PIN, OUTPUT);
    pinMode(EXTERNAL_LED_PIN, OUTPUT);
    digitalWrite(INTEGRATED_LED_PIN, LOW);
    digitalWrite(EXTERNAL_LED_PIN, LOW);
    bt.begin(9600);
    // END BLUETOOTH SETUP




    Wire.begin();





    printf("SDA: %d\n", SDA);
    printf("SCL: %d\n", SCL);

    int recording = 0;
    int reading_n = 0;
    while (1) {
        recording = controller_loop(recording);
        if (!recording)
            reading_n = 0;
        else
            reading_n++;
        

        imu_loop();
        // TODO: travel type has been deleted since it was useless
        // TODO: calculate the following:
        int travel;
        float average_travel;
        int max_travel;

        display_loop();
        //bluetooth_loop();
        sleep_ms(5);
    }

    return 0;
}
