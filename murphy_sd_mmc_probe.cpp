#include <Arduino.h>
#include <SD_MMC.h>

namespace {
constexpr int SDMMC_CLK = 16;
constexpr int SDMMC_CMD = 17;
constexpr int SDMMC_D0 = 15;
constexpr int SDMMC_D1 = 14;
constexpr int SDMMC_D2 = 21;
constexpr int SDMMC_D3 = 18;

void listRoot() {
  File root = SD_MMC.open("/");
  if (!root) {
    Serial.println("open / failed");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("/ is not a directory");
    return;
  }

  Serial.println("root files:");
  File file = root.openNextFile();
  while (file) {
    Serial.printf("  %s %s %llu\n",
                  file.isDirectory() ? "DIR " : "FILE",
                  file.name(),
                  static_cast<unsigned long long>(file.size()));
    file = root.openNextFile();
  }
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(2500);

  Serial.println("Murphy M3 SDMMC probe: OEM recovered 4-bit pins");
  Serial.printf("CLK=%d CMD=%d D0=%d D1=%d D2=%d D3=%d\n",
                SDMMC_CLK, SDMMC_CMD, SDMMC_D0, SDMMC_D1, SDMMC_D2, SDMMC_D3);

  SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0, SDMMC_D1, SDMMC_D2, SDMMC_D3);

  for (int freq : {4000, 1000, 400}) {
    Serial.printf("try SD_MMC 4-bit freq=%d kHz\n", freq);
    if (SD_MMC.begin("/sd", false, false, freq, 5)) {
      Serial.printf("mounted: type=%d size=%llu MB\n",
                    SD_MMC.cardType(),
                    static_cast<unsigned long long>(SD_MMC.cardSize() / (1024ULL * 1024ULL)));
      listRoot();
      return;
    }
    SD_MMC.end();
    delay(250);
  }

  Serial.println("4-bit failed; trying 1-bit control on same CLK/CMD/D0");
  SD_MMC.setPins(SDMMC_CLK, SDMMC_CMD, SDMMC_D0);
  for (int freq : {4000, 1000, 400}) {
    Serial.printf("try SD_MMC 1-bit freq=%d kHz\n", freq);
    if (SD_MMC.begin("/sd", true, false, freq, 5)) {
      Serial.printf("mounted: type=%d size=%llu MB\n",
                    SD_MMC.cardType(),
                    static_cast<unsigned long long>(SD_MMC.cardSize() / (1024ULL * 1024ULL)));
      listRoot();
      return;
    }
    SD_MMC.end();
    delay(250);
  }

  Serial.println("SD_MMC probe failed");
}

void loop() {
  delay(5000);
  Serial.println("holding awake");
}
