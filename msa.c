#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

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

Adafruit_LSM6DSOX sox_fork;
Adafruit_LSM6DSOX sox_frame;
imu_type imu_fork_top;
imu_type imu_fork_bottom;

void imu_loop() {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    sox_fork.getEvent(&accel, &gyro, &temp);

    imu_fork_top.temperature = temp.temperature;
    printf("\t\tTemperature top: %.3f deg C\n", imu_fork_top.temperature);

    /* Display the results (acceleration is measured in m/s^2) */
    imu_fork_top.accel_x = accel.acceleration.x;
    imu_fork_top.accel_y = accel.acceleration.y;
    imu_fork_top.accel_z = accel.acceleration.z;
    printf("\t\tFork top accel X: %.3f\tY: %.3f\tZ: %.3fm/s^2\n", imu_fork_top.accel_x, imu_fork_top.accel_y, imu_fork_top.accel_z);

    /* Display the results (rotation is measured in rad/s) */
    imu_fork_top.gyro_x = gyro.gyro.x;
    imu_fork_top.gyro_y = gyro.gyro.y;
    imu_fork_top.gyro_z = gyro.gyro.z;
    printf("\t\tFork top gyro X: %.3f\tY: %.3f\tZ: %.3fradians/s\n", imu_fork_top.gyro_x, imu_fork_top.gyro_y, imu_fork_top.gyro_z);

    sox_frame.getEvent(&accel, &gyro, &temp);

    imu_fork_bottom.temperature = temp.temperature;
    printf("\t\tTemperature bottom: %.3f deg C\n", imu_fork_bottom.temperature);

    /* Display the results (acceleration is measured in m/s^2) */
    imu_fork_bottom.accel_x = accel.acceleration.x;
    imu_fork_bottom.accel_y = accel.acceleration.y;
    imu_fork_bottom.accel_z = accel.acceleration.z;
    printf("\t\tFork bottom accel X: %.3f\tY: %.3f\tZ: %.3fm/s^2\n", imu_fork_bottom.accel_x, imu_fork_bottom.accel_y, imu_fork_bottom.accel_z);

    /* Display the results (rotation is measured in rad/s) */
    imu_fork_bottom.gyro_x = gyro.gyro.x;
    imu_fork_bottom.gyro_y = gyro.gyro.y;
    imu_fork_bottom.gyro_z = gyro.gyro.z;
    printf("\t\tFork bottom gyro X: %.3f\tY: %.3f\tZ: %.3fradians/s\n", imu_fork_bottom.gyro_x, imu_fork_bottom.gyro_y, imu_fork_bottom.gyro_z);
}

// -------- DISPLAY CODE --------
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Address 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
uint8_t timer = 0;
uint8_t change_info = 0;

void display_loop(uint8_t recording, float average_travel, float max_travel) {
    display.clearDisplay();
    display.setCursor(0, 12);
    if (recording) {
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
            display.print(average_travel);
        }
        if (change_info) {
            display.print("Max: ");
            display.print(max_travel);
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

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}


// -------- MAIN --------
int main() {
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);


    // Timer example code - This example fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);




    puts("Hello, world!");





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
        display.clearDisplay();
        display.setCursor(0, 12);
        display.print("TOP IMU FORK ERR");
        display.display();
        sleep_ms(500);
    }
    while (!sox_frame.begin_I2C(0x6B)) {
        display.clearDisplay();
        display.setCursor(0, 12);
        display.print("BOTTOM IMU FORK ERR");
        display.display();
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
    float travel = 0;
    float average_travel = 0;
    float max_travel = 0;
    while (1) {
        recording = controller_loop(recording);
        if (!recording)
            reading_n = 0;
        else
            reading_n++;
        

        imu_loop();
        // TODO: travel type has been deleted since it was useless
        // TODO: calculate the following:
        travel = 5;
        average_travel = travel / reading_n;
        if (travel > max_travel)
            max_travel = travel;

        display_loop(recording, average_travel, max_travel);
        //bluetooth_loop();
        sleep_ms(500); // change to 5 and comment all printf after sensors testing
        //sleep_ms(5);
    }

    return 0;
}
