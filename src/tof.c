#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;

int n = 0;
int tot = 0;

void tof_setup() {
    Wire.begin();

    sensor.setTimeout(0);
    while (!sensor.init()) {
        Serial.println("TOF error");
        delay(500);
    }

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensor.startContinuous(100);
}

float tof_loop(bool recording) {
    if (!recording) {
        n = 0;
        tot = 0;
    }
    int travel = sensor.readRangeContinuousMillimeters();
    Serial.print("Fork travel: ");
    Serial.println(travel);
    if (sensor.timeoutOccurred() || travel >= 8190) {
        Serial.println("TOF error");
        return 0;
    }

    return travel;
}
