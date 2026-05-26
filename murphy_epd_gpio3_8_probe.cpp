#include <Arduino.h>

constexpr const char* VERSION = "murphy_epd_gpio3_8_probe_v3_oem_init";

constexpr uint8_t EPD_MOSI = 3;
constexpr uint8_t EPD_SCK = 4;
constexpr uint8_t EPD_CS = 5;
constexpr uint8_t EPD_DC = 6;
constexpr uint8_t EPD_RST = 7;
constexpr uint8_t EPD_BUSY = 8;
constexpr uint8_t FRONTLIGHT_PIN = 48;

constexpr int EPD_WIDTH = 240;
constexpr int EPD_HEIGHT = 416;
constexpr int FRAME_BYTES = EPD_WIDTH / 8 * EPD_HEIGHT;

struct InitMode {
  const char* name;
  bool oemRecovered;
  uint8_t panelSetting;
  uint8_t vcomAndDataInterval;
  uint8_t powerOptimize;
};

struct PatternMode {
  const char* name;
  uint8_t oldByte;
  uint8_t (*newByteForOffset)(int byteIndex);
};

uint8_t patternWhite(int) {
  return 0xFF;
}

uint8_t patternBlack(int) {
  return 0x00;
}

uint8_t patternRows(int byteIndex) {
  const int row = byteIndex / (EPD_WIDTH / 8);
  return ((row / 24) % 2 == 0) ? 0x00 : 0xFF;
}

uint8_t patternColumns(int byteIndex) {
  const int colByte = byteIndex % (EPD_WIDTH / 8);
  return (colByte % 2 == 0) ? 0x00 : 0xFF;
}

constexpr InitMode INIT_MODES[] = {
    // Recovered from the OEM display driver decompile.
    {"oem_recovered", true, 0xFF, 0x97, 0x00},
    // Values from the Good Display FT21 sample notes.
    {"ft21_sample", false, 0x1B, 0x97, 0x32},
    // Earlier CrowPanel-style hypothesis.
    {"crowpanel_style", false, 0x1B, 0xD7, 0x6E},
};

constexpr PatternMode PATTERNS[] = {
    {"force_white", 0x00, patternWhite},
    {"force_black", 0xFF, patternBlack},
    {"row_stripes", 0xFF, patternRows},
    {"column_stripes", 0xFF, patternColumns},
};

void spiWrite(uint8_t value) {
  for (int bit = 7; bit >= 0; --bit) {
    digitalWrite(EPD_SCK, LOW);
    digitalWrite(EPD_MOSI, (value >> bit) & 1);
    delayMicroseconds(1);
    digitalWrite(EPD_SCK, HIGH);
    delayMicroseconds(1);
  }
}

void writeCommand(uint8_t command) {
  digitalWrite(EPD_CS, LOW);
  digitalWrite(EPD_DC, LOW);
  spiWrite(command);
  digitalWrite(EPD_CS, HIGH);
}

void writeData(uint8_t data) {
  digitalWrite(EPD_CS, LOW);
  digitalWrite(EPD_DC, HIGH);
  spiWrite(data);
  digitalWrite(EPD_CS, HIGH);
}

void writeDataRun(uint8_t data, int count) {
  digitalWrite(EPD_CS, LOW);
  digitalWrite(EPD_DC, HIGH);
  for (int i = 0; i < count; ++i) {
    spiWrite(data);
  }
  digitalWrite(EPD_CS, HIGH);
}

void writeCommandData(uint8_t command, const uint8_t* data, size_t len) {
  writeCommand(command);
  digitalWrite(EPD_CS, LOW);
  digitalWrite(EPD_DC, HIGH);
  for (size_t i = 0; i < len; ++i) {
    spiWrite(data[i]);
  }
  digitalWrite(EPD_CS, HIGH);
}

bool waitBusyReadyHigh(const char* label, uint32_t timeoutMs) {
  const uint32_t start = millis();
  const int first = digitalRead(EPD_BUSY);
  while (digitalRead(EPD_BUSY) == LOW && millis() - start < timeoutMs) {
    delay(10);
  }
  const int last = digitalRead(EPD_BUSY);
  const bool ok = last == HIGH;
  Serial.printf("[%lu] wait %s: %lu ms busy %d->%d %s\n", millis(), label, millis() - start, first, last,
                ok ? "ready-high" : "timeout");
  return ok;
}

void resetPanel() {
  Serial.printf("[%lu] reset pulse on GPIO%u\n", millis(), EPD_RST);
  digitalWrite(EPD_RST, HIGH);
  delay(20);
  digitalWrite(EPD_RST, LOW);
  delay(20);
  digitalWrite(EPD_RST, HIGH);
  delay(200);
  waitBusyReadyHigh("after reset", 3000);
}

void initUc8253(const InitMode& mode) {
  Serial.printf("[%lu] UC8253 init: %s panel=0x%02X border=0x%02X powerOpt=0x%02X\n", millis(), mode.name,
                mode.panelSetting, mode.vcomAndDataInterval, mode.powerOptimize);

  if (mode.oemRecovered) {
    const uint8_t pwr[] = {0x03, 0x10, 0x3F, 0x3B, 0x0D};
    const uint8_t booster[] = {0xD7, 0xD7, 0x1F};
    const uint8_t resolution[] = {0xF0, 0x01, 0xA0};

    writeCommandData(0x01, pwr, sizeof(pwr));
    writeCommandData(0x06, booster, sizeof(booster));

    Serial.printf("[%lu] OEM init power on\n", millis());
    writeCommand(0x04);
    waitBusyReadyHigh("oem init power on", 5000);

    writeCommand(0x00);
    writeData(0xFF);

    writeCommand(0x30);
    writeData(0x09);

    writeCommandData(0x61, resolution, sizeof(resolution));

    writeCommand(0x82);
    writeData(0x0F);

    writeCommand(0x50);
    writeData(0x97);

    delay(50);
    waitBusyReadyHigh("oem init", 3000);
    return;
  }

  writeCommand(0x00);  // panel setting
  writeData(mode.panelSetting);

  writeCommand(0xE0);
  writeData(0x02);

  writeCommand(0xE5);
  writeData(mode.powerOptimize);

  writeCommand(0x50);
  writeData(mode.vcomAndDataInterval);

  delay(50);
  waitBusyReadyHigh("init", 3000);
}

void writeFrames(const PatternMode& pattern) {
  Serial.printf("[%lu] writing old frame 0x%02X: %d bytes\n", millis(), pattern.oldByte, FRAME_BYTES);
  writeCommand(0x10);
  writeDataRun(pattern.oldByte, FRAME_BYTES);

  Serial.printf("[%lu] writing new frame pattern=%s: %d bytes\n", millis(), pattern.name, FRAME_BYTES);
  writeCommand(0x13);
  digitalWrite(EPD_CS, LOW);
  digitalWrite(EPD_DC, HIGH);
  for (int i = 0; i < FRAME_BYTES; ++i) {
    spiWrite(pattern.newByteForOffset(i));
  }
  digitalWrite(EPD_CS, HIGH);
}

void refresh(const InitMode& init, const PatternMode& pattern) {
  if (init.oemRecovered) {
    Serial.printf("[%lu] power already on from OEM init for %s/%s\n", millis(), init.name, pattern.name);
  } else {
    Serial.printf("[%lu] power on for %s/%s\n", millis(), init.name, pattern.name);
    writeCommand(0x04);
    waitBusyReadyHigh("power on", 5000);
  }

  Serial.printf("[%lu] OBSERVE NOW: refresh init=%s pattern=%s pins MOSI3 SCK4 CS5 DC6 RST7 BUSY8\n", millis(),
                init.name, pattern.name);
  writeCommand(0x12);
  waitBusyReadyHigh("display refresh", 12000);
  delay(2500);

  Serial.printf("[%lu] power off\n", millis());
  writeCommand(0x02);
  waitBusyReadyHigh("power off", 5000);
}

void configurePins() {
  pinMode(FRONTLIGHT_PIN, INPUT);

  pinMode(EPD_MOSI, OUTPUT);
  pinMode(EPD_SCK, OUTPUT);
  pinMode(EPD_CS, OUTPUT);
  pinMode(EPD_DC, OUTPUT);
  pinMode(EPD_RST, OUTPUT);
  pinMode(EPD_BUSY, INPUT);

  digitalWrite(EPD_MOSI, LOW);
  digitalWrite(EPD_SCK, HIGH);
  digitalWrite(EPD_CS, HIGH);
  digitalWrite(EPD_DC, HIGH);
  digitalWrite(EPD_RST, HIGH);
}

void setup() {
  Serial.begin(115200);
  delay(2500);

  Serial.printf("[%lu] Murphy EPD GPIO3-8 probe boot: %s\n", millis(), VERSION);
  Serial.println("Pin map: MOSI=3 SCK=4 CS=5 DC=6 RST=7 BUSY=8 ready-high.");
  Serial.println("GPIO48 is confirmed frontlight and is left INPUT/off.");

  configurePins();
  Serial.printf("[%lu] initial BUSY=%d\n", millis(), digitalRead(EPD_BUSY));

  for (const auto& init : INIT_MODES) {
    for (const auto& pattern : PATTERNS) {
      Serial.println();
      Serial.printf("[%lu] === CASE init=%s pattern=%s ===\n", millis(), init.name, pattern.name);
      resetPanel();
      initUc8253(init);
      writeFrames(pattern);
      refresh(init, pattern);
      Serial.printf("[%lu] case done; pause before next case\n", millis());
      delay(5000);
    }
  }

  Serial.printf("[%lu] probe done; holding awake. BUSY=%d\n", millis(), digitalRead(EPD_BUSY));
}

void loop() {
  delay(5000);
  Serial.printf("[%lu] idle BUSY=%d\n", millis(), digitalRead(EPD_BUSY));
}
