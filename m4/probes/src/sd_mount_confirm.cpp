// Murphy M4 SD mount confirmation probe v2.
// Pin assignments from factory firmware binary (setPins call at VA 0x42008052):
//   CLK=16, CMD=15, D0=17, D1=18, D2=11, D3=14
// Power enable: GPIO10 — testing BOTH polarities (binary analysis may have it backwards).
//
// Also reads SD cluster pin states before/after power toggle to confirm SD_VCC switches.

#include <Arduino.h>
#include <SD_MMC.h>
#include <driver/gpio.h>

#define SD_CLK  16
#define SD_CMD  15
#define SD_D0   17
#define SD_D1   18
#define SD_D2   11
#define SD_D3   14
#define SD_PWR  10

static const int SD_CLUSTER[] = {11, 14, 15, 16, 17, 18};

static void drivePin(int pin, int level) {
    gpio_config_t c = {};
    c.pin_bit_mask  = (1ULL << pin);
    c.mode          = GPIO_MODE_OUTPUT;
    c.pull_up_en    = GPIO_PULLUP_DISABLE;
    c.pull_down_en  = GPIO_PULLDOWN_DISABLE;
    c.intr_type     = GPIO_INTR_DISABLE;
    gpio_config(&c);
    gpio_set_level((gpio_num_t)pin, level);
}

static void readCluster(const char* label) {
    Serial.printf("  SD cluster @ %s:", label);
    for (int p : SD_CLUSTER) {
        gpio_config_t c = {};
        c.pin_bit_mask = (1ULL << p);
        c.mode = GPIO_MODE_INPUT;
        c.pull_up_en = GPIO_PULLUP_DISABLE;
        c.pull_down_en = GPIO_PULLDOWN_DISABLE;
        c.intr_type = GPIO_INTR_DISABLE;
        gpio_config(&c);
        Serial.printf(" G%d=%d", p, gpio_get_level((gpio_num_t)p));
    }
    Serial.println();
}

static bool tryMount(int pwr_level) {
    SD_MMC.end();
    gpio_reset_pin((gpio_num_t)SD_CLK);
    gpio_reset_pin((gpio_num_t)SD_CMD);
    gpio_reset_pin((gpio_num_t)SD_D0);
    gpio_reset_pin((gpio_num_t)SD_D1);
    gpio_reset_pin((gpio_num_t)SD_D2);
    gpio_reset_pin((gpio_num_t)SD_D3);
    delay(50);

    Serial.printf("\n--- GPIO10 = %s ---\n", pwr_level ? "HIGH" : "LOW");
    drivePin(SD_PWR, pwr_level);
    delay(200);
    readCluster("after pwr");

    // 4-bit
    SD_MMC.setPins(SD_CLK, SD_CMD, SD_D0, SD_D1, SD_D2, SD_D3);
    for (int freq : {400, 1000, 4000}) {
        Serial.printf("  4-bit @ %d kHz... ", freq);
        if (SD_MMC.begin("/sd", false, false, freq, 5)) {
            Serial.println("MOUNTED!");
            Serial.printf("  Size: %llu MB\n", SD_MMC.cardSize() / (1024*1024));
            File root = SD_MMC.open("/");
            File f = root.openNextFile(); int n = 0;
            while (f && n++ < 10) {
                Serial.printf("    %s %s\n", f.isDirectory()?"DIR":"FILE", f.name());
                f = root.openNextFile();
            }
            return true;
        }
        Serial.println("failed");
        SD_MMC.end(); delay(15);
    }

    // 1-bit
    gpio_reset_pin((gpio_num_t)SD_CLK);
    gpio_reset_pin((gpio_num_t)SD_CMD);
    gpio_reset_pin((gpio_num_t)SD_D0);
    SD_MMC.setPins(SD_CLK, SD_CMD, SD_D0);
    for (int freq : {400, 1000}) {
        Serial.printf("  1-bit @ %d kHz... ", freq);
        if (SD_MMC.begin("/sd", true, false, freq, 5)) {
            Serial.println("MOUNTED (1-bit)!");
            return true;
        }
        Serial.println("failed");
        SD_MMC.end(); delay(15);
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("\n=== Murphy M4 SD mount confirm v2 ===");
    Serial.println("CLK=16 CMD=15 D0=17 D1=18 D2=11 D3=14 PWR=GPIO10 (both polarities)");

    readCluster("baseline");

    if (tryMount(1)) {
        Serial.println("\n*** SUCCESS: GPIO10 HIGH = SD powered ***");
        return;
    }

    gpio_reset_pin((gpio_num_t)SD_PWR);
    delay(300);

    if (tryMount(0)) {
        Serial.println("\n*** SUCCESS: GPIO10 LOW = SD powered ***");
        return;
    }

    Serial.println("\n=== Both polarities failed. Binary pin map may be wrong. ===");
}

void loop() { delay(10000); Serial.println("idle"); }
