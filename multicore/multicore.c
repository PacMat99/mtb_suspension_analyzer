#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#define LED_PIN 25

// Multicore flag value
#define GPIO_ON 1
#define GPIO_OFF 0

void second_core_code() {
    while (1) {
        sleep_ms(500);
        multicore_fifo_push_blocking(GPIO_ON);
        sleep_ms(500);
        multicore_fifo_push_blocking(GPIO_OFF);
    }
}

int main() {
    stdio_init_all();

    printf("Hello, multicore!\n");

    // launch core 1 (second core)
    multicore_launch_core1(second_core_code);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while(1) {
        uint32_t command = multicore_fifo_pop_blocking();
        gpio_put(LED_PIN, command);
    }

    return 0;
}
