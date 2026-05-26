# Murphy M3 Flash Dump Findings

Input: `m3_flash_dump.bin`  
Size: `0x1000000` bytes / 16 MiB  
SHA256: `a755d15262c684b3cf8c9bee0a221996811320208a9fd4366c5d6ecc821f94d6`

## High Confidence Summary

- Target platform is Espressif `ESP32-S3`.
- Firmware format is ESP-IDF app image format, not DOS COM despite the `file` utility guess.
- Flash layout is 16 MiB with bootloader, NVS, OTA metadata, one populated app slot, one erased app slot, an erased 2 MiB filesystem partition, and an erased coredump partition.
- Active app is `app0` at flash offset `0x010000`; `app1` at `0x6e0000` is all `0xff`.
- App metadata reports `ESP-IDF v4.4.7-dirty`, project name `arduino-lib-builder`, compile time `Mar 5 2024 12:12:53`.
- Build uses Arduino-ESP32 under PlatformIO paths from a Windows user profile: `C:/Users/HZW/.platformio/...`.
- Firmware identifies as an e-paper / ink-screen reader and embeds a Gitee update URL for `corogoo/3.7-inch-ink-screen-reader`.
- Network setup exposes a local web UI with WiFi, weather, and OTA update endpoints.
- The physical unit has a headphone jack, and firmware includes an I2S DAC/codec audio stack with music/audiobook playback evidence.
- OEM touch logic is present (`touchTask`, touch-area reset UI, touch-specific OTA URL). Murphy-specific Ghidra work now points the controller path at a CHSC6x/Chipsemi-class I2C device on `SDA=GPIO13`, `SCL=GPIO12`, address `0x2e`, using command bytes `0xa3`/`0xa5` and a 5-byte sample path. `GPIO44` is confirmed as the touch INT/wake line by passive probing, and `GPIO45` is the strongest reset/control candidate. The earlier `0x38` ACK on the same bus returns static invalid FT-style data and is now treated as a false/stale candidate unless future tap-correlated data proves otherwise.
- OEM button logic includes long-press handling, held-direction acceleration, custom key mapping, key-map reset, and key-swap support. The actual Murphy unit has no rotary button. Custom passive input probing confirms three active-low side buttons: top `GPIO1`, middle `GPIO2`, and bottom `GPIO0`; CrowPanel GPIO4/GPIO5/GPIO6 input assumptions are reference-only until verified. No explicit hidden debug/verbose multi-button combo has been recovered yet.
- OEM front-light UI is present (`Front Light`) and custom firmware confirms the Murphy front light is controlled by `GPIO48`, active high, with 25 kHz PWM working cleanly.
- OEM SD card code uses Arduino `SD_MMC` in 4-bit mode. Recovered pins are `CLK=GPIO16`, `CMD=GPIO17`, `D0=GPIO15`, `D1=GPIO14`, `D2=GPIO21`, `D3=GPIO18`. The older `GPIO39/13/40/10` SPI tuple is now a superseded public-reference lead.
- Stock firmware emits application-level logs over USB, including observed `#ClickLock` and `>>>Count:%d Time:%dms` messages. A hidden verbosity flag is possible but not yet proven.
- The firmware includes clock, calendar, alarm, and NTP synchronization features. The HamGeek M3 listing claims an independent clock chip; an external RTC is plausible but not yet proven.
- NVS contains readable WiFi configuration, including SSID/password-like values. Treat `analysis/extracted/nvs.bin` as sensitive.
- Ghidra 12.1 has been set up with Xtensa support and a project was created at `analysis/ghidra-project/MurphyM3`.
- Ghidra GUI is not required for the current workflow. Headless Ghidra runs successfully via `/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless`; see `oem_headless_ghidra.md`.

## Output Map

- `findings/flash_layout.md`: partition table, extracted files, hashes.
- `findings/firmware_identity.md`: ESP-IDF/app metadata and build provenance.
- `findings/hardware.md`: inferred hardware features and confidence.
- `findings/display_bringup.md`: UC8253 display bring-up, pin/power probe matrix, BUSY polarity tests, and OEM LUT evidence.
- `findings/display_gpio_recovery.md`: OEM-derived display GPIO tuple and init/update command recovery.
- `findings/display_oem_driver_recovery.md`: deeper Ghidra function/dataflow notes for GPIO3-8, init, LUT loading, and refresh.
- `findings/display_lut_refresh.md`: extracted default/alternate OEM LUT tables and practical `0x10`/`0x13`/`0x12` refresh sequence.
- `findings/touch.md`: OEM touch strings, controller candidates, custom probe results, and SDK integration plan.
- `findings/button_input.md`: physical key map, OEM long-press/custom-key evidence, and combo test plan.
- `findings/front_light.md`: OEM front-light settings evidence, external panel lead, confirmed `GPIO48` PWM control, and SDK implementation notes.
- `findings/sd_card_gpio.md`: OEM-recovered Murphy 4-bit SDMMC mapping, superseded SPI lead, and next probe code.
- `findings/usb_logging.md`: confirmed stock USB app logs, capture workflow, and verbosity/debug-flag search notes.
- `findings/online_research.md`: web-sourced product, panel, and module clues.
- `findings/audio.md`: headphone-jack/audio playback evidence, codec unknowns, and porting impact.
- `findings/clock_rtc.md`: clock, NTP, alarms, internal RTC evidence, and external RTC test plan.
- `findings/network_and_ui.md`: web endpoints, OTA, WiFi/weather UI.
- `findings/storage_and_nvs.md`: NVS and filesystem observations.
- `findings/ghidra.md`: Ghidra/Xtensa setup and analysis artifacts.
- `findings/oem_headless_ghidra.md`: headless Ghidra commands, OEM hardware-mining scripts, and current static-analysis limits.
- `findings/porting_crosspoint.md`: feasibility and work breakdown for porting `community-sdk` and `crosspoint-reader-main`.
- `analysis/`: raw extracted partitions, strings, segments, Ghidra logs.
- `_m3_flash_dump.bin.extracted/esp-partitions/`: requested binwalk-style extraction directory populated with ESP partitions.
