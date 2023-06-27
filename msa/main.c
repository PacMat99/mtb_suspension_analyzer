#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "display.h"
#include "bluetooth.h"
#include "controller.h"
#include "tof.h"
#include "imu.h"

int main() {
    stdio_init_all();
    display_setup();
    bluetooth_setup();
    controller_setup();
    tof_setup();
    imu_setup();

    while (1) {
        bool recording = controller_loop();
        //bool recording = true;
        bool error = false;
        float average_travel = tof_loop(recording);
        if (!average_travel || !imu_loop(recording))
            error = true;
        display_loop(recording, error);
        bluetooth_loop(average_travel);
    }

    return 0;
}
