// Murphy M4 — test untested HIGH-baseline pins GPIO1 and GPIO2 as SD power enable.
//
// GPIO26-32 on ESP32-S3R8 are the internal flash/PSRAM SPI bus — NOT accessible as GPIO.
// Configuring them crashes the chip. Don't touch them.
//
// The gap found: gpio_state showed GPIO1=HIGH and GPIO2=HIGH at baseline,
// but sd_power_detect's PWR_CANDIDATES hardcoded list omitted them.
// They have never been tested as active-LOW (PMOS) SD power enable.
//
// This probe:
//   Phase A: GPIO1 LOW, GPIO2 LOW — watch SD cluster for LOW->HIGH (PMOS power enable).
//   Phase B: GPIO1 HIGH, GPIO2 HIGH — watch for NMOS (unlikely but cover it).
//   If SD cluster rises >= 4 pins on any config, immediately try SD_MMC mount.

#include <Arduino.h>
#include <SD_MMC.h>
#include <driver/gpio.h>

static const int SD_CLUSTER[] = {11, 14, 15, 16, 17, 18};
static const int N_SD = 6;

static const int TEST_PINS[] = {1, 2};
static const int N_TEST = 2;

static const int WATCH_PINS[] = {
    1, 2, 3, 5, 6, 7, 8, 9,
    11, 12, 13, 14, 15, 16, 17, 18, 21,
    38, 39, 40, 41, 42, 43, 44, 46, 47, 48
};
static const int N_WATCH = sizeof(WATCH_PINS)/sizeof(WATCH_PINS[0]);

static int baseline[40];
static int s_prev[6] = {-1,-1,-1,-1,-1,-1};

static void configInput(int pin) {
    gpio_config_t c = {};
    c.pin_bit_mask = (1ULL << pin);
    c.mode = GPIO_MODE_INPUT;
    c.pull_up_en = GPIO_PULLUP_DISABLE;
    c.pull_down_en = GPIO_PULLDOWN_DISABLE;
    c.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&c);
}

static void configOutput(int pin, int level) {
    gpio_config_t c = {};
    c.pin_bit_mask = (1ULL << pin);
    c.mode = GPIO_MODE_OUTPUT;
    c.pull_up_en = GPIO_PULLUP_DISABLE;
    c.pull_down_en = GPIO_PULLDOWN_DISABLE;
    c.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&c);
    gpio_set_level((gpio_num_t)pin, level);
}

static void allInputs() {
    for (int i = 0; i < N_WATCH; i++) configInput(WATCH_PINS[i]);
}

static void readAll(int out[]) {
    for (int i = 0; i < N_WATCH; i++)
        out[i] = gpio_get_level((gpio_num_t)WATCH_PINS[i]);
}

static void tryMount(int pwr_pin, int pwr_level) {
    Serial.println("  SD cluster rose! Attempting SD_MMC mount...");
    static const int SD_PINS[] = {11, 14, 15, 16, 17, 18};
    static const int N_SP = 6;

    for (int ci = 0; ci < N_SP; ci++) {
        int clk = SD_PINS[ci];
        for (int ai = 0; ai < N_SP; ai++) {
            int cmd = SD_PINS[ai];
            if (cmd == clk || cmd == pwr_pin) continue;
            for (int bi = 0; bi < N_SP; bi++) {
                int d0 = SD_PINS[bi];
                if (d0 == clk || d0 == cmd || d0 == pwr_pin) continue;

                for (int i = 0; i < 3; i++) {
                    if (s_prev[i] >= 0) { gpio_reset_pin((gpio_num_t)s_prev[i]); s_prev[i] = -1; }
                }
                SD_MMC.end();
                s_prev[0] = clk; s_prev[1] = cmd; s_prev[2] = d0;
                delay(50);
                SD_MMC.setPins(clk, cmd, d0);
                for (int freq : {400, 1000}) {
                    if (SD_MMC.begin("/sd", true, false, freq, 5)) {
                        Serial.printf("*** MOUNTED! PWR=GPIO%d(%s) CLK=%d CMD=%d D0=%d @ %dkHz ***\n",
                            pwr_pin, pwr_level?"HIGH":"LOW", clk, cmd, d0, freq);
                        Serial.println("Holding — write these down!");
                        while(true){ delay(5000); Serial.println("holding..."); }
                    }
                    SD_MMC.end(); delay(10);
                }
            }
        }
    }
    Serial.println("  Mount failed for all pin combos with this power config.");
}

static void testPin(int pin, int driveLevel) {
    Serial.printf("--- GPIO%d -> drive %s ---\n", pin, driveLevel ? "HIGH" : "LOW");
    allInputs();
    delay(50);
    configOutput(pin, driveLevel);
    delay(300);

    int after[40];
    for (int i = 0; i < N_WATCH; i++) {
        if (WATCH_PINS[i] == pin) { after[i] = driveLevel; continue; }
        configInput(WATCH_PINS[i]);
        after[i] = gpio_get_level((gpio_num_t)WATCH_PINS[i]);
    }

    int rose = 0, sd_rose = 0;
    for (int i = 0; i < N_WATCH; i++) {
        if (WATCH_PINS[i] == pin) continue;
        if (baseline[i] == 0 && after[i] == 1) {
            Serial.printf("  GPIO%d: LOW->HIGH\n", WATCH_PINS[i]);
            rose++;
            for (int s : SD_CLUSTER)
                if (WATCH_PINS[i] == s) sd_rose++;
        } else if (baseline[i] == 1 && after[i] == 0) {
            Serial.printf("  GPIO%d: HIGH->LOW\n", WATCH_PINS[i]);
        }
    }
    if (rose == 0) Serial.println("  (no changes)");
    if (sd_rose >= 3) tryMount(pin, driveLevel);

    gpio_reset_pin((gpio_num_t)pin);
    allInputs();
    delay(400);
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(2500);
    Serial.println("\n=== Murphy M4 GPIO1+GPIO2 power enable test ===");
    Serial.println("GPIO1 and GPIO2 were HIGH at baseline but never tested as SD power enable.\n");

    allInputs();
    delay(100);
    readAll(baseline);

    Serial.print("GPIO1="); Serial.println(baseline[0] ? "HIGH" : "LOW");
    Serial.print("GPIO2="); Serial.println(baseline[1] ? "HIGH" : "LOW");
    Serial.println();

    Serial.println("=== Phase A: drive LOW (active-low/PMOS power enable) ===\n");
    for (int p : TEST_PINS) testPin(p, 0);

    Serial.println("=== Phase B: drive HIGH (active-high, less likely) ===\n");
    for (int p : TEST_PINS) testPin(p, 1);

    Serial.println("=== Done. ===");
}

void loop() { delay(10000); Serial.println("idle"); }
