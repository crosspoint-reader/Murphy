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
- OEM touch logic is present (`touchTask`, touch-area reset UI, touch-specific OTA URL). Online panel research makes `FT6336U` the leading controller candidate, but the exact touch I2C pins remain unproven.
- OEM front-light UI is present (`Front Light`). Online panel research points to a 9-LED front-light assembly, but the Murphy GPIO/PWM/driver path remains unproven.
- The firmware includes clock, calendar, alarm, and NTP synchronization features. The HamGeek M3 listing claims an independent clock chip; an external RTC is plausible but not yet proven.
- NVS contains readable WiFi configuration, including SSID/password-like values. Treat `analysis/extracted/nvs.bin` as sensitive.
- Ghidra 12.1 has been set up with Xtensa support and a project was created at `analysis/ghidra-project/MurphyM3`.

## Output Map

- `findings/flash_layout.md`: partition table, extracted files, hashes.
- `findings/firmware_identity.md`: ESP-IDF/app metadata and build provenance.
- `findings/hardware.md`: inferred hardware features and confidence.
- `findings/touch.md`: OEM touch strings, controller candidates, routing evidence, and SDK integration plan.
- `findings/front_light.md`: OEM front-light settings evidence, external panel lead, likely driver shape, and pin/PWM unknowns.
- `findings/online_research.md`: web-sourced product, panel, and module clues.
- `findings/audio.md`: headphone-jack/audio playback evidence, codec unknowns, and porting impact.
- `findings/clock_rtc.md`: clock, NTP, alarms, internal RTC evidence, and external RTC test plan.
- `findings/network_and_ui.md`: web endpoints, OTA, WiFi/weather UI.
- `findings/storage_and_nvs.md`: NVS and filesystem observations.
- `findings/ghidra.md`: Ghidra/Xtensa setup and analysis artifacts.
- `findings/porting_crosspoint.md`: feasibility and work breakdown for porting `community-sdk` and `crosspoint-reader-main`.
- `analysis/`: raw extracted partitions, strings, segments, Ghidra logs.
- `_m3_flash_dump.bin.extracted/esp-partitions/`: requested binwalk-style extraction directory populated with ESP partitions.
