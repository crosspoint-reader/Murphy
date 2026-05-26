# Murphy M3 SD Card GPIO Mapping

Created: 2026-05-26

Scope: Murphy M3 only. Do not mix this with Xteink X3/X4 SD or display pin maps.

## Practical Answer

The OEM firmware is using Arduino `SD_MMC`, not the `SD.h`/SdFat SPI path.

Recovered OEM SDMMC mapping:

| SDMMC signal | GPIO | Status | Notes |
| --- | ---: | --- | --- |
| `CLK` | `16` | OEM recovered | First argument to the OEM `setPins` wrapper. |
| `CMD` | `17` | OEM recovered | Second argument to the OEM `setPins` wrapper. |
| `D0` | `15` | OEM recovered | Third argument. Required in 1-bit and 4-bit modes. |
| `D1` | `14` | OEM recovered | Fourth argument. OEM uses 4-bit mode, so this is required. |
| `D2` | `21` | OEM recovered | Fifth argument. OEM uses 4-bit mode, so this is required. |
| `D3` | `18` | OEM recovered | Sixth argument. OEM uses 4-bit mode, so this is required. |

Minimal Arduino SDMMC probe:

```cpp
#include <Arduino.h>
#include <SD_MMC.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  SD_MMC.setPins(16, 17, 15, 14, 21, 18);

  if (!SD_MMC.begin("/sd", false, false, 4000, 5)) {
    Serial.println("SD_MMC 4-bit mount failed");
    return;
  }

  Serial.printf("SD_MMC mounted: type=%d size=%llu MB\n",
                SD_MMC.cardType(),
                SD_MMC.cardSize() / (1024ULL * 1024ULL));
}

void loop() {}
```

Start at `4000` kHz. The OEM passes a firmware-selected frequency value, but the pin/mode recovery is the important part: `mode1bit=false`, max-open-files `5`.

## Superseded SPI Lead

The earlier public CrowPanel-style SPI tuple is still useful as a historical lead, but it is not what the Murphy OEM firmware is doing:

| SPI-style label | GPIO | Current status | Notes |
| --- | ---: | --- | --- |
| `SCK` / `CLK` | `39` | Superseded for Murphy OEM path | Public docs and existing port notes list this, but CrossPoint/SdFat hangs here. |
| `MISO` | `13` | Superseded for Murphy OEM path | Public docs list this. |
| `MOSI` | `40` | Superseded for Murphy OEM path | Public docs list this. |
| `CS` | `10` | Superseded for Murphy OEM path | Public docs list this. |

The old Arduino SPI probe was:

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

This should no longer be the primary Murphy probe. It may describe a reference carrier or another board variant, but the OEM binary points at native 4-bit SDMMC on GPIO16/17/15/14/21/18.

## Confidence And Caveat

The OEM SDMMC tuple is statically recovered but not yet live-confirmed in our custom firmware. Treat it as the next bounded probe, not final truth until an SD card mounts and lists files.

The older SPI tuple came from official CrowPanel 3.7-inch source that labels the SD slot as SPI:

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

The concrete OEM callsite has now been recovered:

```c
FUN_4202b864(_DAT_41f40194, 10, 0);
FUN_4202b698(uVar3, 0x10, 0x11, 0x0f, 0x0e, 0x15, 0x12);
FUN_4202b6c4(uVar3, _DAT_41f401a8, 0, 0, uVar4, 5);
```

Interpretation:

- `FUN_4202b698` is the OEM wrapper for `SD_MMC.setPins(clk, cmd, d0, d1, d2, d3)`.
- It stores the six byte pin values at object offsets `0x0c..0x11`.
- `FUN_4202b6c4` is the OEM wrapper for `SD_MMC.begin(mountpoint, mode1bit, format_if_mount_failed, sdmmc_frequency, maxOpenFiles)`.
- The third argument to `FUN_4202b6c4` is `0`, so `mode1bit=false` and the OEM uses 4-bit SDMMC.
- The sixth argument is `5`, matching Arduino `SD_MMC.begin(..., maxOpenFiles=5)`.
- The recovered pin tuple is decimal `16,17,15,14,21,18`.

Evidence:

- `analysis/oem_hardware_constant_mining.md:354811-354842`
- `analysis/oem_hardware_constant_mining.md:385267-385318`
- `analysis/oem_hardware_constant_mining.md:385293-385368`
- `analysis/oem_hardware_keyword_mining.md:241055-241086`

## If SPI Fails

The SPI probe already hangs in CrossPoint's SdFat path. The next test should be native SDMMC:

1. Try OEM 4-bit SDMMC:

```cpp
#include <Arduino.h>
#include <SD_MMC.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  SD_MMC.setPins(16, 17, 15, 14, 21, 18);

  if (!SD_MMC.begin("/sd", false, false, 4000, 5)) {
    Serial.println("SD_MMC 4-bit mount failed");
    return;
  }

  Serial.printf("SD_MMC mounted: type=%d size=%llu MB\n",
                SD_MMC.cardType(),
                SD_MMC.cardSize() / (1024ULL * 1024ULL));
}

void loop() {}
```

2. If that fails, try the same pins in 1-bit mode as a control:

```cpp
SD_MMC.setPins(16, 17, 15);
SD_MMC.begin("/sd", true, false, 4000, 5);
```

3. If both fail, reduce frequency to `1000` and then `400` kHz before revisiting power sequencing.

## Current Recommendation

Use the OEM SDMMC tuple first:

```text
CLK=16
CMD=17
D0=15
D1=14
D2=21
D3=18
mode1bit=false
```

Once an SD card mounts and lists files, mark these as confirmed in this file and in `findings/codex_handoff.md`.
