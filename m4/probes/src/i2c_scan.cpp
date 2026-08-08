// Murphy M4 I2C scanner — sweeps likely SDA/SCL pin pairs and reports all devices found.
// Historical broad bus scan. The 0x38 response found by this probe did not
// yield the working touch path; see findings/touch.md for the confirmed bus.
// Retained to document the original discovery process; it is not needed for
// building or installing the completed port.
#include <Arduino.h>
#include <Wire.h>
#include <esp_log.h>
#include <driver/gpio.h>

// GPIO pins to exclude: USB (19/20), flash (26-32), BOOT button (0), USB strapping (45/46)
// Also skip 3 (JTAG TX used for serial on some configs) and the SD probe will cover higher pins.
// We focus on the range most likely to carry I2C on a compact e-reader PCB.
static const uint8_t CANDIDATE_PINS[] = {
    1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    21, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 47, 48
};
static const int NUM_CANDIDATES = sizeof(CANDIDATE_PINS) / sizeof(CANDIDATE_PINS[0]);

static const char* knownDevice(uint8_t addr) {
    switch (addr) {
        case 0x38: return "unidentified response (not confirmed touch path)";
        case 0x2E: return "touch-controller address (confirm wiring separately)";
        case 0x5D: return "GT911 touch (addr A)";
        case 0x14: return "GT911 touch (addr B)";
        case 0x55: return "BQ27220 fuel gauge";
        case 0x10: return "ES8388 audio codec";
        case 0x18: return "ES8311 audio codec";
        case 0x51: return "PCF8563 RTC";
        case 0x68: return "DS3231 RTC";
        default:   return nullptr;
    }
}

static bool scanPair(uint8_t sda, uint8_t scl) {
    Wire.end();
    gpio_pullup_en((gpio_num_t)sda);
    gpio_pullup_en((gpio_num_t)scl);
    Wire.begin(sda, scl, 100000);
    Wire.setTimeOut(2);
    delay(5);

    bool anyFound = false;
    for (uint8_t addr = 0x08; addr < 0x78; addr++) {
        Wire.beginTransmission(addr);
        uint8_t err = Wire.endTransmission();
        if (err == 0) {
            const char* name = knownDevice(addr);
            if (name) {
                Serial.printf("  *** SDA=%-2d SCL=%-2d addr=0x%02X  %s\n", sda, scl, addr, name);
            } else {
                Serial.printf("      SDA=%-2d SCL=%-2d addr=0x%02X\n", sda, scl, addr);
            }
            anyFound = true;
        }
    }
    Wire.end();
    return anyFound;
}

void setup() {
    Serial.begin(115200);
    esp_log_level_set("i2c.master", ESP_LOG_NONE);
    delay(2500);
    Serial.println("\n=== Murphy M4 I2C Scanner ===");
    Serial.printf("Scanning %d candidate pins in pairs...\n", NUM_CANDIDATES);
    Serial.println("Lines marked *** are known interesting devices.");
    Serial.println("Note: working touch is GPIO13/GPIO12 at 0x2E.\n");

    int pairsScanned = 0;
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        for (int j = 0; j < NUM_CANDIDATES; j++) {
            if (i == j) continue;
            uint8_t sda = CANDIDATE_PINS[i];
            uint8_t scl = CANDIDATE_PINS[j];
            scanPair(sda, scl);
            pairsScanned++;
        }
        // Progress heartbeat every SDA pin
        Serial.printf("[%lus] SDA=%d done (%d pairs so far)\n",
                      millis() / 1000, CANDIDATE_PINS[i], pairsScanned);
    }

    Serial.println("\n=== Scan complete. ===");
    Serial.println("Compare any responses with the reviewed hardware findings.");
}

void loop() {
    delay(10000);
    Serial.println("idle — scan already printed above");
}
