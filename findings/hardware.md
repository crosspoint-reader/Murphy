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
- Front light/backlight: the HamGeek M3 listing claims a 10-level backlight/front-light system. Good Display's matching 3.7-inch touch/front-light panel family lists a front-light variant.

## Likely / Medium Confidence

- Product lineage/codebase: the firmware references `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json`, so this image appears derived from or directly built from the Corogoo 3.7-inch ink-screen reader firmware.
- Touch controller: likely an external capacitive controller. Online panel research makes `FT6336U` the best current candidate, but the exact Murphy part and GPIO pins are not proven. The public CrowPanel schematic names display FPC pins `TSCL`/`TSDA`, while the matching public board file does not route U4 pads 6/7. See `touch.md`.
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
