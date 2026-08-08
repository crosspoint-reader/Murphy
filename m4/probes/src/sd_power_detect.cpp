// Murphy M4 SD power detect probe.
// SD CMD/D0-D3 pull-ups connect to SD_VCC (switched rail), not main 3V3.
// When SD power is off, those lines read LOW. When power turns on, they float HIGH.
// Strategy: drive each HIGH pin LOW one at a time (potential PMOS power enable),
// wait for SD_VCC to ramp, read all GPIOs again. Any pin that flips LOW→HIGH
// is an SD data/cmd line. 5 pins rising = we found the power pin and data pins.
#include <Arduino.h>
#include <driver/gpio.h>

static const uint8_t ALL_PINS[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 21,
    38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48
};
static const int N_ALL = sizeof(ALL_PINS);

// Pins that read HIGH in the initial gpio_state probe — candidates for power enable
static const int PWR_CANDIDATES[] = {8, 9, 12, 13, 39, 42, 43, 45};
static const int N_PWR = sizeof(PWR_CANDIDATES) / sizeof(PWR_CANDIDATES[0]);

static int baseline[50];

static void readAll(int out[]) {
    for (int i = 0; i < N_ALL; i++)
        out[i] = gpio_get_level((gpio_num_t)ALL_PINS[i]);
}

static void configAllInputs() {
    for (int i = 0; i < N_ALL; i++) {
        gpio_config_t cfg = {};
        cfg.pin_bit_mask  = (1ULL << ALL_PINS[i]);
        cfg.mode          = GPIO_MODE_INPUT;
        cfg.pull_up_en    = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en  = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type     = GPIO_INTR_DISABLE;
        gpio_config(&cfg);
    }
}

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\n=== Murphy M4 SD Power Detect Probe ===");
    Serial.println("Driving each HIGH pin LOW to find the SD_VCC power switch.");
    Serial.println("SD data/cmd lines will flip LOW→HIGH when SD_VCC turns on.\n");

    configAllInputs();
    delay(100);
    readAll(baseline);

    Serial.println("Baseline GPIO state:");
    for (int i = 0; i < N_ALL; i++)
        Serial.printf("  GPIO%-2d = %s\n", ALL_PINS[i], baseline[i] ? "HIGH" : "LOW ");
    Serial.println();

    for (int pi = 0; pi < N_PWR; pi++) {
        int pwr = PWR_CANDIDATES[pi];
        Serial.printf("--- Testing GPIO%d as SD_POWER enable (drive LOW) ---\n", pwr);

        // Drive candidate LOW (PMOS gate → transistor on → SD_VCC on)
        gpio_config_t cfg = {};
        cfg.pin_bit_mask  = (1ULL << pwr);
        cfg.mode          = GPIO_MODE_OUTPUT;
        cfg.pull_up_en    = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en  = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type     = GPIO_INTR_DISABLE;
        gpio_config(&cfg);
        gpio_set_level((gpio_num_t)pwr, 0);

        delay(250);  // let SD_VCC ramp and card stabilize

        // Re-read all as inputs (except pwr pin)
        int powered[50];
        for (int i = 0; i < N_ALL; i++) {
            if (ALL_PINS[i] == (uint8_t)pwr) {
                powered[i] = 0;  // we're driving it, not reading
                continue;
            }
            gpio_config_t ic = {};
            ic.pin_bit_mask  = (1ULL << ALL_PINS[i]);
            ic.mode          = GPIO_MODE_INPUT;
            ic.pull_up_en    = GPIO_PULLUP_DISABLE;
            ic.pull_down_en  = GPIO_PULLDOWN_DISABLE;
            ic.intr_type     = GPIO_INTR_DISABLE;
            gpio_config(&ic);
            powered[i] = gpio_get_level((gpio_num_t)ALL_PINS[i]);
        }

        // Show changes
        int rose = 0;
        for (int i = 0; i < N_ALL; i++) {
            if (ALL_PINS[i] == (uint8_t)pwr) continue;
            if (baseline[i] == 0 && powered[i] == 1) {
                Serial.printf("  GPIO%-2d: LOW→HIGH  *** SD data/cmd candidate!\n", ALL_PINS[i]);
                rose++;
            } else if (baseline[i] == 1 && powered[i] == 0) {
                Serial.printf("  GPIO%-2d: HIGH→LOW  (unexpected)\n", ALL_PINS[i]);
            }
        }
        if (rose == 0) {
            Serial.println("  (no changes — not the power pin)");
        } else if (rose == 5) {
            Serial.printf("\n*** GPIO%d IS the SD power enable! "
                          "The %d pins above are SD CMD+D0-D3. ***\n", pwr, rose);
        } else {
            Serial.printf("  (%d pin(s) rose — partial match, note these)\n", rose);
        }

        // Release power pin, let SD_VCC discharge
        gpio_reset_pin((gpio_num_t)pwr);
        configAllInputs();
        delay(500);
        Serial.println();
    }

    Serial.println("=== Phase A done (PMOS/active-low). Now trying NMOS/active-high: drive LOW pins HIGH ===\n");

    // Phase B: drive each LOW pin HIGH — covers NMOS or load-switch (active-high enable)
    // Exclude known pins: I2C SCL=4, I2C SDA=10, frontlight=47/48, Touch_INT=44
    static const int LOW_CANDIDATES[] = {3, 5, 6, 7, 11, 14, 15, 16, 17, 18, 21, 38, 40, 41, 46};
    static const int N_LOW = sizeof(LOW_CANDIDATES) / sizeof(LOW_CANDIDATES[0]);

    for (int pi = 0; pi < N_LOW; pi++) {
        int pwr = LOW_CANDIDATES[pi];
        Serial.printf("--- Testing GPIO%d as SD_POWER enable (drive HIGH) ---\n", pwr);

        gpio_config_t cfg = {};
        cfg.pin_bit_mask  = (1ULL << pwr);
        cfg.mode          = GPIO_MODE_OUTPUT;
        cfg.pull_up_en    = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en  = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type     = GPIO_INTR_DISABLE;
        gpio_config(&cfg);
        gpio_set_level((gpio_num_t)pwr, 1);  // HIGH → NMOS on → SD_VCC enabled

        delay(250);

        int powered[50];
        for (int i = 0; i < N_ALL; i++) {
            if (ALL_PINS[i] == (uint8_t)pwr) { powered[i] = 1; continue; }
            gpio_config_t ic = {};
            ic.pin_bit_mask  = (1ULL << ALL_PINS[i]);
            ic.mode          = GPIO_MODE_INPUT;
            ic.pull_up_en    = GPIO_PULLUP_DISABLE;
            ic.pull_down_en  = GPIO_PULLDOWN_DISABLE;
            ic.intr_type     = GPIO_INTR_DISABLE;
            gpio_config(&ic);
            powered[i] = gpio_get_level((gpio_num_t)ALL_PINS[i]);
        }

        int rose = 0;
        for (int i = 0; i < N_ALL; i++) {
            if (ALL_PINS[i] == (uint8_t)pwr) continue;
            if (baseline[i] == 0 && powered[i] == 1) {
                Serial.printf("  GPIO%-2d: LOW→HIGH  *** SD data/cmd candidate!\n", ALL_PINS[i]);
                rose++;
            } else if (baseline[i] == 1 && powered[i] == 0) {
                Serial.printf("  GPIO%-2d: HIGH→LOW  (unexpected)\n", ALL_PINS[i]);
            }
        }
        if (rose == 0)
            Serial.println("  (no changes)");
        else if (rose == 5)
            Serial.printf("\n*** GPIO%d IS the SD power enable (active HIGH)! %d pins above = SD CMD+D0-D3 ***\n", pwr, rose);
        else
            Serial.printf("  (%d pin(s) rose)\n", rose);

        gpio_reset_pin((gpio_num_t)pwr);
        configAllInputs();
        delay(500);
        Serial.println();
    }

    Serial.println("=== All phases done. ===");
}

void loop() {
    delay(10000);
    Serial.println("idle");
}
