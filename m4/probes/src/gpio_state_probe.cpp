// Murphy M4 GPIO state probe.
// Reads all GPIO values as high-impedance inputs at boot, before touching anything.
// SD card CMD and D0-D3 lines have hardware PCB pull-ups → will read HIGH.
// SD CLK has no pull-up → will read LOW or float.
// I2C SDA/SCL (GPIO10/4) should also read HIGH (confirmed I2C pull-ups).
// This identifies which GPIOs are physically connected to the SD slot.
#include <Arduino.h>
#include <driver/gpio.h>

// All testable GPIOs (exclude flash 26-32, PSRAM 33-37, USB 19-20)
static const uint8_t TEST_PINS[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 21,
    38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48
};

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\n=== Murphy M4 GPIO State Probe ===");
    Serial.println("Reading all GPIOs as floating inputs.");
    Serial.println("HIGH = has pull-up (SD CMD/D0-D3, I2C, buttons)");
    Serial.println("LOW  = no pull-up (SD CLK, display signals, etc.)\n");

    int n = sizeof(TEST_PINS);

    // First pass: configure all as input with NO software pull-up/down
    for (int i = 0; i < n; i++) {
        gpio_config_t cfg = {};
        cfg.pin_bit_mask = (1ULL << TEST_PINS[i]);
        cfg.mode = GPIO_MODE_INPUT;
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_DISABLE;
        gpio_config(&cfg);
    }

    delay(50); // settle

    // Read all
    Serial.println("GPIO  VALUE  Notes");
    Serial.println("----  -----  -----");
    for (int i = 0; i < n; i++) {
        int pin = TEST_PINS[i];
        int val = gpio_get_level((gpio_num_t)pin);
        const char* note = "";
        if (pin == 0)  note = "(BOOT button — probably LOW if not pressed)";
        if (pin == 4)  note = "(I2C SCL — expect HIGH)";
        if (pin == 10) note = "(I2C SDA — expect HIGH)";
        if (pin == 47) note = "(frontlight cool)";
        if (pin == 48) note = "(frontlight warm)";
        Serial.printf("GPIO%-2d  %s     %s\n", pin, val ? "HIGH" : "LOW ", note);
    }

    Serial.println("\n=== Interpretation ===");
    Serial.println("GPIOs reading HIGH (beyond known I2C/buttons) are likely:");
    Serial.println("  SD CMD, D0, D1, D2, D3 — all have PCB pull-ups");
    Serial.println("GPIOs reading LOW are likely:");
    Serial.println("  SD CLK, display signals, or unconnected pins");
}

void loop() {
    delay(10000);
    Serial.println("idle");
}
