#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
//#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
//#include "ssd1306_font.h"
#include "ssd1306_display/ssd1306_display.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
//#define SPI_PORT spi0
//#define PIN_MISO 16
//#define PIN_CS   17
//#define PIN_SCK  18
//#define PIN_MOSI 19

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
//#define I2C_PORT i2c0
//#define I2C_SDA 8
//#define I2C_SCL 9

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

int main() {
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    //spi_init(SPI_PORT, 1000*1000);
    //gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    //gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    //gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    //gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    //gpio_set_dir(PIN_CS, GPIO_OUT);
    //gpio_put(PIN_CS, 1);
    

    // I2C Initialisation. Using it at 400Khz.
    //i2c_init(I2C_PORT, 400*1000);
    
    //gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    //gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    //gpio_pull_up(I2C_SDA);
    //gpio_pull_up(I2C_SCL);


    // Timer example code - This example fires off the callback after 2000ms
    //add_alarm_in_ms(2000, alarm_callback, NULL, false);

    printf("Hello, SSD1306 OLED display! Look at my raspberries..\n");

    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // run through the complete initialization process
    SSD1306_init();

    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area frame_area = {
        start_col: 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
        };

    calc_render_area_buflen(&frame_area);

    uint8_t buf[SSD1306_BUF_LEN];

restart:

    //SSD1306_scroll(true);

    sleep_ms(500);

    // zero the entire display
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    char *waiting[] = {
        "WAITING FOR",
        "A GREAT RIDE"
    };

    int i, y;
    for (i = 0, y = 0; i < count_of(waiting); i++) {
        WriteString(buf, 5, y, waiting[i]);
        y += 20;
    }
    render(buf, &frame_area);

    sleep_ms(5000);

    // zero the entire display
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    char *recording[] = {
        "RECORDING",
        "HAPPY RIDE"
    };

    y = 0;
    for (i = 0, y = 0; i < count_of(recording); i++) {
        WriteString(buf, 5, y, recording[i]);
        y += 20;
    }
    render(buf, &frame_area);

    sleep_ms(5000);

    goto restart;

    return 0;
}
