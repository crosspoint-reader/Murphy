# Hardware Inferences

## High Confidence

- MCU/chipset: Espressif `ESP32-S3`.
- External flash: 16 MiB, DIO mode, 80 MHz.
- Wireless: WiFi and BLE stacks are included.
- Storage: SD/MMC support is included. Strings include `SDCard mounted at: %s`, `SDCard sector count`, `SD_MMC.cpp`, `/cdcard`, `/FONT`.
- Filesystem: LittleFS support is included and mounted at `/littlefs`.
- Input: touch handling exists. Strings include `touchTask`, `<Reset custom touch area>`, and `Touch to exit`. See `touch.md`.
- Physical buttons: custom passive input probing confirms three active-low side buttons: top `GPIO1`, middle `GPIO2`, and bottom `GPIO0`. See `button_input.md`.
- Display class: e-paper / ink screen reader. Strings include `E-Paper Reader`, `Ink screen reader`, `3.7-inch-ink-screen-reader`, and ePub reader UI strings.
- Audio output connector: the physical Murphy M3 unit has a headphone jack.
- Front light/backlight: the OEM firmware includes a `Front Light` setting, and custom firmware confirms active-high PWM control on `GPIO48`. A 25 kHz, 10-bit PWM probe visibly adjusted brightness. See `front_light.md`.

## Likely / Medium Confidence

- Product lineage/codebase: the firmware references `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json`, so this image appears derived from or directly built from the Corogoo 3.7-inch ink-screen reader firmware.
- Touch controller: likely an external CHSC6x/Chipsemi-class capacitive controller. Current OEM RE and live probes point to `SDA=GPIO13`, `SCL=GPIO12`, address `0x2e`, and command bytes `0xa3`/`0xa5`. `GPIO44` is confirmed as the active-low touch INT/wake line. The calibrated coordinate decode is `raw_x=byte4`, `raw_y=(byte5 << 8) | byte6`, mapped from about `x=24..224` and `y=24..392` into the 416x240 screen. `GPIO45` is the strongest reset/control candidate but should not be used in normal firmware yet. The earlier `0x38` ACK on `SDA=13/SCL=12` returns static invalid FT-style data and is not a usable touch path. See `touch.md`.
- Front-light driver: confirmed board-level control input on `GPIO48`; the LED current path is still assumed to be a transistor/driver rather than direct GPIO LED current. See `front_light.md`.
- External RTC: the HamGeek M3 listing claims a built-in independent clock chip. Firmware evidence has not yet revealed an RTC part number or I2C address. See `clock_rtc.md`.
- Audio implementation details: `ESP32-audioI2S-3.0.12` is present, including MP3/AAC/FLAC/Opus/Vorbis decoder strings, I2S driver strings, `musicTask`, and `Connect to DAC codec...`. Given the observed headphone jack, the best current read is an external I2S DAC/codec path, but the codec IC and pins are still unknown. See `audio.md`.
- PSRAM is likely expected by some features: strings include `SPIRAM`, `SPI RAM enabled but initialization failed`, and audio/playlist warnings about PSRAM. Presence of these strings can come from libraries, so this is not proof of fitted PSRAM.

## Display / Panel

Evidence points to a 3.7-inch e-paper display, but the exact panel controller is not fully proven from strings alone.

Relevant strings:

- `E-Paper Reader`
- `Ink screen reader`
- `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/...`
- `$GC9`

`$GC9` may indicate a GC9-series display-controller dependency or signature, but the surrounding binary context is not enough to assert the exact display controller. No clear `ILI`, `ST77`, `SSD`, or Waveshare panel identifier was recovered from the first string pass.

Custom firmware display bring-up now produces visible output through the OEM-derived `MOSI3/SCK4/CS5/DC6/RST7/BUSY8` display bus. `GPIO48` is confirmed as front-light PWM on this Murphy unit, so public mappings that use `GPIO48` as EPD `BUSY` are invalid here. The remaining display work is UC8253 refresh quality, LUT/plane handling, and UI/rendering polish on the GPIO3-8 map. See `display_bringup.md`.

The panel is Good Display `GDEY037T03-FT21`: 3.7-inch, UC8253, 416x240, SPI display interface, touch, and front light. The visible FPC markings `YSFK082048A-W-3` / `LH37004D 2513` appear to be module/FPC production markings for this assembly.

## User-Facing Features

The firmware appears to implement:

- ePub reading: `.epub`, `src/epub_parser.cpp`, `Could not find <spine> tag`, `readerPageDecodeEpub`.
- Fonts: `/FONT`, `/FONT/%s.font`, `>>>Font count`.
- JPEG/image handling: `jpeg`, `Not supported JPEG standard`, `Jpeg file not found`.
- Weather display/configuration.
- Clock/time sync, alarms, and calendar UI. See `clock_rtc.md`.
- Lock-screen wallpaper/content.
- Bluetooth scan/connect flow.
- SD card file listing.
