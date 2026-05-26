# Hardware Inferences

## High Confidence

- MCU/chipset: Espressif `ESP32-S3`.
- External flash: 16 MiB, DIO mode, 80 MHz.
- Wireless: WiFi and BLE stacks are included.
- Storage: SD/MMC support is included. Strings include `SDCard mounted at: %s`, `SDCard sector count`, `SD_MMC.cpp`, `/cdcard`, `/FONT`.
- Filesystem: LittleFS support is included and mounted at `/littlefs`.
- Input: touch handling exists. Strings include `touchTask`, `<Reset custom touch area>`, and `Touch to exit`. See `touch.md`.
- Display class: e-paper / ink screen reader. Strings include `E-Paper Reader`, `Ink screen reader`, `3.7-inch-ink-screen-reader`, and ePub reader UI strings.
- Audio output connector: the physical Murphy M3 unit has a headphone jack.
- Front light/backlight: the OEM firmware includes a `Front Light` setting. The HamGeek M3 listing claims a 10-level backlight/front-light system, and Good Display's matching 3.7-inch touch/front-light panel family lists a 9-LED front-light variant. See `front_light.md`.

## Likely / Medium Confidence

- Product lineage/codebase: the firmware references `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json`, so this image appears derived from or directly built from the Corogoo 3.7-inch ink-screen reader firmware.
- Touch controller: likely an external capacitive controller. Online panel research makes `FT6336U` the best current candidate, but the exact Murphy part and GPIO pins are not proven. The public CrowPanel schematic names display FPC pins `TSCL`/`TSDA`, while the matching public board file does not route U4 pads 6/7. See `touch.md`.
- Front-light driver: likely an external LED-control path driven by GPIO and/or PWM. The firmware links LEDC/PWM and GPIO functions, but no recovered xref yet ties a specific channel or pin to the `Front Light` setting. See `front_light.md`.
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

Custom firmware display bring-up has not yet changed the panel. A standalone probe using the public CrowPanel UC8253 pin sets and vendor-style bit-banged writes leaves GPIO48 BUSY low and produces no visible update. The vendor sample archive for `GDEY037T03-FT21` uses a different ESP32 adapter-board control-pin mapping (`CS=A17/DC=A16/RST=A15/BUSY=A14`) and a different fast-init sequence (`0xE5=0x32`, `0x50=0x97`). Under the current ESP32-S3 Arduino variant those aliases map to `CS18/DC17/RST16/BUSY15`, which is now included in the standalone probe. Treat all board-level display GPIO mappings as unconfirmed for this Murphy unit until OEM signal capture proves them. See `display_bringup.md`.

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
