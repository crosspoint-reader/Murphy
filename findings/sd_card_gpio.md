# Murphy M3 SD Card GPIO Mapping

Created: 2026-05-26

Scope: Murphy M3 only. Do not mix this with Xteink X3/X4 SD or display pin maps.

## Practical Answer

Use this SD-card mapping first:

| SD signal | GPIO | Status | Notes |
| --- | ---: | --- | --- |
| `SCK` / `CLK` | `39` | High-confidence vendor mapping | Public Elecrow/CrowPanel 3.7-inch docs and existing port notes agree. |
| `MISO` | `13` | High-confidence vendor mapping | In SD native terminology this is likely `D0`. |
| `MOSI` | `40` | High-confidence vendor mapping | In SD native terminology this is likely `CMD`. |
| `CS` | `10` | High-confidence vendor mapping | In SD native terminology this may correspond to `D3`/card-select behavior. |

First Arduino SPI probe:

```cpp
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

constexpr int SD_SCK = 39;
constexpr int SD_MISO = 13;
constexpr int SD_MOSI = 40;
constexpr int SD_CS = 10;

SPIClass sdSpi(FSPI);

void setup() {
  Serial.begin(115200);
  delay(1000);

  sdSpi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  if (!SD.begin(SD_CS, sdSpi, 4000000, "/sd", 5)) {
    Serial.println("SD mount failed");
    return;
  }

  Serial.printf("SD mounted: type=%d size=%llu MB\n",
                SD.cardType(),
                SD.cardSize() / (1024ULL * 1024ULL));
}

void loop() {}
```

The same probe has been preserved as:

- `murphy_sd_spi_probe.cpp`

Start at `4 MHz`. If it mounts reliably, test higher frequencies later. Do not spend time on SD power-enable guesses first; no Murphy-specific SD power pin is confirmed yet.

## Confidence And Caveat

This SD tuple is not yet live-confirmed on Murphy in the same way the display GPIO3-8 tuple is. Treat it as the first bounded probe, not final truth until an SD card mounts and lists files.

The reason it is still the best first try is that the official CrowPanel 3.7-inch source labels the SD slot as SPI and gives the same four pins:

```text
MOSI GPIO40
MISO GPIO13
CLK  GPIO39
CS   GPIO10
```

Local evidence:

- `findings/online_research.md:44-45`
- `findings/porting_crosspoint.md:67-70`
- `findings/porting_crosspoint.md:125-128`

Web evidence:

- Elecrow wiki for CrowPanel ESP32 E-Paper HMI 3.7-inch lists `SD Card Slot(SPI) | MOSI(IO40); MISO(IO13); CLK(IO39); CS(IO10)`.

## OEM Firmware Evidence

The OEM app definitely has SD-card support:

- Strings include `Initializing SD card`, `SDCard mounted at: %s`, `SDCard sector count`, `No SD card`, and `SD card read error`.
- The app links Arduino `SD_MMC`/`SDMMCFS` code as well as card/mount error handling.

Evidence:

- `analysis/app0.strings.txt:495-538`
- `analysis/app0.strings.txt:8236-8240`
- `findings/hardware.md:8`

The headless Ghidra pass found an OEM SD card mount class around `FUN_42013ca8`.

Key points from the decompile:

- `FUN_42013ca8` logs `Initializing SD card`.
- It copies an SDMMC slot config template and overwrites the first six slot-pin fields from constructor parameters.
- It calls a mount function, then logs `SDCard mounted at: %s` and `SDCard sector count: %d, size: %d` on success.
- `FUN_42013f34` wraps this and creates an `SDCardWriter` task.

Evidence:

- `analysis/sd_literal_refs_20260526.md:292-379`
- `analysis/sd_constructor_refs_20260526.md:28-93`
- `analysis/sd_constructor_refs_20260526.md:232-271`

The six OEM slot-config parameters line up with Arduino `SD_MMC.setPins(clk, cmd, d0, d1, d2, d3)` and ESP-IDF `sdmmc_slot_config_t` field order.

Local API evidence:

- `/Users/patryk/.platformio/packages/framework-arduinoespressif32/libraries/SD_MMC/src/SD_MMC.cpp:47-74`
- `/Users/patryk/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/driver/include/driver/sdmmc_host.h:54-82`
- `/Users/patryk/.platformio/packages/framework-arduinoespressif32/tools/sdk/esp32s3/include/driver/include/driver/sdmmc_host.h:93-108`

Unresolved OEM detail: the actual callsite supplying those six SDMMC parameters to `FUN_420140a4`/`FUN_42013f34` has not been recovered yet. The Ghidra pass found the SD mount class and wrapper, but not the concrete runtime arguments.

## If SPI Fails

If the SPI probe above does not mount:

1. Try lower SPI speeds first: `1 MHz`, then `400 kHz`.
2. Try native one-bit SDMMC with the three shared signal names:

```cpp
#include <Arduino.h>
#include <SD_MMC.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  // SDMMC terminology for the same likely slot:
  // CLK=39, CMD=40, D0=13.
  SD_MMC.setPins(39, 40, 13);

  if (!SD_MMC.begin("/sd", true, false, 4000, 5)) {
    Serial.println("SD_MMC 1-bit mount failed");
    return;
  }

  Serial.printf("SD_MMC mounted: type=%d size=%llu MB\n",
                SD_MMC.cardType(),
                SD_MMC.cardSize() / (1024ULL * 1024ULL));
}

void loop() {}
```

3. Do not random-sweep GPIOs. The next useful static RE task is to recover the caller that passes six parameters into `FUN_420140a4` or inspect an actual Murphy schematic/board trace for SD `D1`/`D2` if OEM native 4-bit SDMMC is required.

## Current Recommendation

Use SPI first:

```text
SCK=39
MISO=13
MOSI=40
CS=10
```

Once an SD card mounts and lists files, mark these as confirmed in this file and in `findings/codex_handoff.md`.
