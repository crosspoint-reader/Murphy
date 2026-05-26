# Murphy M3 Reverse Engineering

This repo collects the firmware dump, extracted artifacts, reverse-engineering notes, and porting plan for the Murphy M3 e-paper reader hardware.

## Current Read

The dump is a 16 MiB ESP32-S3 flash image. The active firmware is an ESP-IDF/Arduino build from March 5, 2024, with OTA partitioning and one populated app slot. Strings identify the app as an e-paper / ink-screen reader with WiFi, BLE, SD card, LittleFS, web OTA, weather, fonts, JPEG, audio playback, clock/calendar/alarm features, and ePub reader support.

The hardware most likely matches the Elecrow CrowPanel ESP32 3.7-inch E-paper HMI family:

- ESP32-S3-WROOM-1-N8R8 class module.
- 8 MiB PSRAM.
- 16 MiB flash on this dumped unit.
- 3.7-inch black/white e-paper panel.
- UC8253 display controller.
- 240x416 panel resolution.
- SD card slot on a separate SPI bus.

The physical Murphy M3 unit also has a headphone jack. Firmware evidence points to an I2S DAC/codec audio path, but the codec part and pins are not yet identified. Clock features appear to be NTP/system-time based in firmware, while the HamGeek M3 listing claims a built-in independent clock chip, so an external RTC is now plausible but still unproven at IC/address level.

Touch handling is present in the OEM firmware (`touchTask`, touch-area reset UI, and touch-specific OTA URL), but the exact touch controller and GPIO pins are not yet proven. Online panel research makes Good Display's `FT6336U`-based 3.7-inch touch/front-light module the strongest external match so far. The public CrowPanel schematic names display FPC pins `TSCL`/`TSDA`; the matching public board file does not route those pins, so the Murphy unit needs probing or deeper firmware recovery for touch pin mapping.

Front-light support is also present in the OEM UI as a `Front Light` settings label. Pressing and holding either right-side button on the Murphy unit opens the front-light controls. HamGeek claims 10 brightness levels and Good Display's closest panel-family match lists a 9-LED front-light assembly, but the Murphy GPIO/PWM/driver path is not yet recovered.

## Findings

Start here:

- [Findings index](findings/README.md)

Detailed notes:

- [Flash layout](findings/flash_layout.md): ESP-IDF partition table, extracted partitions, hashes.
- [Firmware identity](findings/firmware_identity.md): app metadata, ESP-IDF/Arduino/PlatformIO evidence, build provenance.
- [Hardware inferences](findings/hardware.md): hardware features recovered from strings and firmware structure.
- [Display bring-up notes](findings/display_bringup.md): UC8253 probe results, failed pin/power matrix, and required OEM signal-capture next steps.
- [Touch hardware and firmware notes](findings/touch.md): OEM touch strings, likely controller families, routing evidence, and SDK port shape.
- [Button input and combo notes](findings/button_input.md): physical key map, OEM long-press/custom-key evidence, and combo test plan.
- [Front light hardware and firmware notes](findings/front_light.md): OEM front-light UI evidence, panel lead, likely driver shape, and open pin/PWM questions.
- [USB logging and console notes](findings/usb_logging.md): confirmed stock USB app logs, capture workflow, and verbosity/debug-flag search notes.
- [Online research notes](findings/online_research.md): product listings and panel references found on the web.
- [Audio hardware and capabilities](findings/audio.md): headphone-jack evidence, I2S audio stack, supported formats, unknown codec/pins.
- [Clock, time sync, and RTC](findings/clock_rtc.md): NTP/timekeeping evidence, alarm UI, RTC unknowns and test plan.
- [Network and UI](findings/network_and_ui.md): web endpoints, OTA behavior, WiFi/weather/UI strings.
- [Storage and NVS](findings/storage_and_nvs.md): NVS contents, filesystem observations, sensitive-data notes.
- [Ghidra setup](findings/ghidra.md): Xtensa support, project setup, and analysis artifacts.
- [CrossPoint porting plan](findings/porting_crosspoint.md): feasibility, board profile, SDK changes, and work breakdown.

## Repository Layout

- `m3_flash_dump.bin`: original 16 MiB flash dump.
- `analysis/extracted/`: carved ESP32 partitions.
- `analysis/ghidra-project/`: Ghidra project created for the app image.
- `analysis/ghidra_inventory.md`: exported Ghidra inventory.
- `analysis/audio_rtc_string_refs.md`: Ghidra string-reference export for audio and clock/RTC terms.
- `analysis/touch_string_refs.md`, `analysis/touch_pointer_refs.md`: Ghidra exports for touch-related strings and pointer/literal-pool references.
- `analysis/button_combo_string_refs.md`, `analysis/button_combo_pointer_refs.md`: Ghidra exports for key-map, long-press, and lock-key strings.
- `analysis/frontlight_string_refs.md`, `analysis/frontlight_pointer_refs.md`, `analysis/frontlight_table_memory.md`: Ghidra exports for front-light settings strings and adjacent pointer tables.
- `analysis/usb_logging_string_refs.md`: Ghidra export for observed USB log strings and log-level-related symbols.
- `analysis/vendor/`: vendor CrowPanel reference material cloned for hardware comparison.
- `_m3_flash_dump.bin.extracted/esp-partitions/`: binwalk-style extraction directory populated with ESP partitions.
- `findings/`: human-readable reverse-engineering and porting notes.
- `tools/ghidra_scripts/`: scripts used to load ESP32-S3 app segments and export inventory.
- `tools/ghidra-xtensa/`: local copy/build of the Xtensa Ghidra processor plugin.

## Flash Layout Summary

The firmware uses ESP-IDF OTA partitioning:

| Partition | Offset | Size | State |
| --- | ---: | ---: | --- |
| `nvs` | `0x009000` | `0x005000` | populated |
| `otadata` | `0x00e000` | `0x002000` | selects `app0` |
| `app0` | `0x010000` | `0x6d0000` | valid active app |
| `app1` | `0x6e0000` | `0x6d0000` | erased |
| `spiffs` | `0xdb0000` | `0x200000` | erased, firmware strings mention LittleFS |
| `coredump` | `0xff0000` | `0x010000` | erased |

## Port Overview

Porting CrossPoint is feasible, but it requires a new board target rather than a simple pin swap.

Required work:

- Add an ESP32-S3 PlatformIO environment for Murphy M3 / CrowPanel 3.7.
- Add a board profile to `community-sdk` with Murphy pins and capabilities.
- Add a UC8253 240x416 display backend to `EInkDisplay`.
- Move SD support to a configurable, separate SPI bus.
- Replace the Xteink ADC-button input path with Murphy digital button input and touch-assisted navigation.
- Add display power sequencing and separate SD SPI support.
- Reserve pins and abstractions for headphone/I2S audio and a possible RTC until hardware mapping is complete.

Minimum viable port:

- ESP32-S3 build boots.
- Full-screen black/white UC8253 refresh works. This is not yet achieved; standalone probes on the public CrowPanel pin sets have not changed the panel.
- SD card mounts and files can be read.
- GPIO1/GPIO2 inputs cover the confirmed physical buttons; GPIO4/GPIO5/GPIO6 are CrowPanel-reference inputs only until proven on Murphy hardware.
- Battery can be stubbed or disabled initially.
- Touch should be added as a target-rectangle event source that invokes the same actions as existing button-driven menu selection, because this Murphy unit has no rotary button.
- Front light can be added later as a board-level brightness peripheral once the GPIO/PWM/driver path is proven.
- Audio and external RTC support can be disabled initially, with board abstractions left in place.
- Partial refresh and grayscale can come after basic display stability.
