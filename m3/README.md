# Murphy M3

Firmware dump, extracted artifacts, reverse-engineering notes, and porting plan for the Murphy M3 e-paper reader hardware.

The dump is a 16 MiB ESP32-S3 flash image. The active firmware is an ESP-IDF/Arduino build from March 5, 2024, identified as an e-paper / ink-screen reader derived from the Corogoo 3.7-inch ink-screen reader. The hardware closely matches the Elecrow CrowPanel ESP32 3.7-inch E-paper HMI family (ESP32-S3-WROOM-1-N8R8 class, 8 MiB PSRAM, 16 MiB flash, UC8253-based Good Display `GDEY037T03-FT21` panel at 416x240).

For the canonical per-subsystem summary, see **[findings/README.md](findings/README.md)**.

## Quick Start

- **Findings (start here):** [findings/README.md](findings/README.md)
- **Porting plan:** [findings/porting_crosspoint.md](findings/porting_crosspoint.md)
- **Display hardware + bus:** [findings/display_hardware.md](findings/display_hardware.md)
- **Display driver + LUTs:** [findings/display_driver.md](findings/display_driver.md)
- **Flash layout:** [findings/flash_layout.md](findings/flash_layout.md)
- **Firmware identity:** [findings/firmware_identity.md](findings/firmware_identity.md)

## Layout (`m3/`)

- `m3_flash_dump.bin` — original 16 MiB flash dump.
- `probes/murphy_epd_gpio3_8_probe.cpp`, `probes/murphy_frontlight_probe.cpp`, `probes/murphy_sd_mmc_probe.cpp` — standalone Arduino probes for the OEM-derived display bus, front-light PWM, and SDMMC pins.
- `findings/` — human-readable reverse-engineering and porting notes.
- `analysis/` — raw extracted partitions, strings, segments, Ghidra exports, and headless decompile outputs.
- `extracted/` — carved ESP32 partitions.
- `ghidra-project/MurphyM3/` — Ghidra project for the app image.
- `binwalk_extracted/esp-partitions/` — binwalk-style extraction directory.

Cross-device material lives outside `m3/`: the MurphyOS reader firmware in [`../murphyos/`](../murphyos/), vendor reference in [`../vendor/`](../vendor/), and shared tooling in [`../tools/`](../tools/).

## Flash Layout

ESP-IDF OTA partitioning on a 16 MiB flash:

| Partition | Offset | Size | State |
| --- | ---: | ---: | --- |
| `nvs` | `0x009000` | `0x005000` | populated |
| `otadata` | `0x00e000` | `0x002000` | selects `app0` |
| `app0` | `0x010000` | `0x6d0000` | valid active app |
| `app1` | `0x6e0000` | `0x6d0000` | erased |
| `spiffs` | `0xdb0000` | `0x200000` | erased; firmware strings mention LittleFS |
| `coredump` | `0xff0000` | `0x010000` | erased |

See [findings/flash_layout.md](findings/flash_layout.md) for extracted partition files and hashes.

## Confirmed Pin Map (Quick Reference)

For the full evidence and per-subsystem notes, follow the linked docs.

| Function | Pins | Doc |
| --- | --- | --- |
| Display bus (bit-banged UC8253) | `MOSI=3 SCK=4 CS=5 DC=6 RST=7 BUSY=8` (ready-high) | [display_hardware.md](findings/display_hardware.md) |
| Front light | `GPIO48`, active-high PWM, 25 kHz | [front_light.md](findings/front_light.md) |
| Side buttons | top `GPIO1`, middle `GPIO2`, bottom `GPIO0` (active-low) | [button_input.md](findings/button_input.md) |
| SD card (4-bit SDMMC) | `CLK=16 CMD=17 D0=15 D1=14 D2=21 D3=18`, drive `GPIO10` low before mount | [sd_card_gpio.md](findings/sd_card_gpio.md) |
| Touch (CHSC6x-class I2C) | `SDA=13 SCL=12`, addr `0x2e`, INT `GPIO44` active-low | [touch.md](findings/touch.md) |

GPIO0/1/2 are buttons and GPIO48 is front-light PWM — exclude these from any display, touch, RTC, or audio probing.

## Port Status

CrossPoint boots on Murphy with display, SD, buttons, and front light working. See [findings/porting_crosspoint.md](findings/porting_crosspoint.md) for the board profile, SDK changes, and remaining work (refresh quality tuning, touch as event source, audio and external RTC still stubbed).
