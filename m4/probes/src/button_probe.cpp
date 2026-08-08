// Murphy M4 button probe.
// GPIO1 and GPIO2 are both HIGH at baseline — likely top and middle side buttons
// with pull-ups. Buttons are typically active-LOW (pressing pulls the line to GND).
// Press each button while watching serial output to confirm which GPIO it is.

#include <Arduino.h>
#include <driver/gpio.h>

static const int CANDIDATES[] = {1, 2, 3, 9, 12, 13, 38, 39, 40, 41, 42, 43, 46};
static const int N = sizeof(CANDIDATES) / sizeof(CANDIDATES[0]);

static int prev[13];

void setup() {
    Serial.begin(115200);
    delay(1500);

    for (int i = 0; i < N; i++) {
        gpio_config_t c = {};
        c.pin_bit_mask  = (1ULL << CANDIDATES[i]);
        c.mode          = GPIO_MODE_INPUT;
        c.pull_up_en    = GPIO_PULLUP_DISABLE;
        c.pull_down_en  = GPIO_PULLDOWN_DISABLE;
        c.intr_type     = GPIO_INTR_DISABLE;
        gpio_config(&c);
        prev[i] = gpio_get_level((gpio_num_t)CANDIDATES[i]);
    }

    Serial.println("\n=== Murphy M4 button probe ===");
    Serial.println("Press each side button. Any GPIO that changes will be reported.");
    Serial.println("(GPIO0 = bottom button, already confirmed)\n");

    Serial.print("Baseline: ");
    for (int i = 0; i < N; i++)
        Serial.printf("G%d=%d ", CANDIDATES[i], prev[i]);
    Serial.println();
}

void loop() {
    for (int i = 0; i < N; i++) {
        int cur = gpio_get_level((gpio_num_t)CANDIDATES[i]);
        if (cur != prev[i]) {
            Serial.printf("GPIO%d: %s  <-- BUTTON?\n",
                CANDIDATES[i], cur ? "HIGH (released)" : "LOW  (pressed) ");
            prev[i] = cur;
        }
    }
    delay(20);
}
