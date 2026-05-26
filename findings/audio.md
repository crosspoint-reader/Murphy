# Audio Hardware And Capabilities

## Summary

The physical Murphy M3 unit has a headphone jack, so this hardware variant has at least one audio output connector. The firmware also contains a substantial audio playback stack, including `ESP32-audioI2S-3.0.12`, decoder sources, playback task strings, I2S driver strings, and `Connect to DAC codec...`.

Best current read: audio output is likely driven by an external I2S DAC or codec feeding the headphone jack. The exact codec IC, I2S pins, headphone detect behavior, mute/power GPIOs, and amplifier topology are not yet recovered from the dump.

The Elecrow CrowPanel ESP32 3.7-inch E-paper HMI schematic used as a board reference does not show a headphone jack, audio codec, headphone amplifier, or audio connector. That means the Murphy M3 is either a modified hardware variant or the available CrowPanel schematic is incomplete for this device.

## Firmware Evidence

The app image links the `ESP32-audioI2S` Arduino library:

- `.pio/libdeps/esp32-s3-devkitc-1/ESP32-audioI2S-3.0.12/src/Audio.cpp`
- `.pio/libdeps/esp32-s3-devkitc-1/ESP32-audioI2S-3.0.12/src/Audio.h`
- `ESP32-audioI2S`
- `Connect to DAC codec...`
- `ERROR: Unable to install I2S drives`
- `BUNINSTALL I2S`
- `musicTask`
- `>>>music_set_volume(%d)`
- `>>>music Task Play Done`

The firmware includes ESP-IDF I2S driver references such as `i2s_driver_install`, `i2s_set_pin`, and `i2s_write`. The string set is consistent with PCM audio leaving the ESP32-S3 over I2S rather than only using Bluetooth or a software-only media parser.

## Supported Formats

The linked audio stack contains decoders and format handling for:

- MP3: `.mp3`, `audio/mpeg`, `audio/mp3`
- AAC/M4A/MP4 audio: `.aac`, `.m4a`, `audio/aac`, `audio/mp4`, `audio/m4a`
- WAV: `.wav`, `audio/wav`
- FLAC: `.flac`, `audio/flac`
- Opus: `.opus`
- Vorbis/Ogg: `audio/ogg`

Decoder source paths in strings include AAC, FLAC, MP3, Opus, and Vorbis components under `ESP32-audioI2S-3.0.12`.

## Playback Sources

The firmware appears to support local files, web streams, playlists, and transport-stream/HLS playback:

- `/music.list`
- `Music`
- `Audiobooks`
- `processLocalFile`
- `processWebFile`
- `processWebStreamHLS`
- `processWebStreamTS`
- `parsePlaylist_M3U8`
- `readPlayListData`
- `playChunk`
- `audio/scpls`
- `audio/mpegurl`
- `m3u8 playlists requires PSRAM enabled!`

This is more than a minimal beep/tone feature. It looks like user-facing music or audiobook playback is compiled in.

## Metadata And Web Audio

Additional strings indicate parsing of audio metadata and internet audio responses:

- `Audio-Length`
- `icy-metaint`
- `StreamTitle`
- `Official audio`
- `official audio`
- `Release time`
- `/v1/audio/speech`

The `/v1/audio/speech` string could come from linked library support or app code. It is evidence of a compiled-in HTTP audio/TTS path, but it is not enough by itself to prove the shipped UI exposes online text-to-speech.

## Vendor Firmware Comparison

The public Corogoo `3.7-inch-ink-screen-reader` repository contains firmware images in the same family. Its `firmware/touch/firmware.bin` has the same key metadata as this dump:

- ESP32-S3 app image
- 16 MiB flash configuration
- ESP-IDF `v4.4.7-dirty`
- compile time `Mar 5 2024 12:12:53`
- project name `arduino-lib-builder`

That image is not byte-identical to `analysis/extracted/app0.bin`, but its strings include the same `ESP32-audioI2S-3.0.12`, I2S, codec, and music task evidence.

The older/non-touch Corogoo firmware has different audio evidence, including `>>>bt_set_volume(%d)`, `BluetoothA2DPSource::start_raw(...)`, `Advanced Audio`, and `DAC channel error`. That suggests the firmware lineage moved between Bluetooth/internal-DAC-style audio paths and the touch/Murphy I2S DAC/codec path.

## What We Do Not Know Yet

- Audio codec or DAC part number.
- Whether the headphone jack is driven by a true headphone amplifier, a line output, or a codec-integrated driver.
- Whether the output is stereo, mono, or mono mixed to stereo.
- I2S BCLK, LRCLK, DOUT, DIN, and MCLK GPIOs.
- Whether the design uses I2C/SPI control for the codec.
- Codec I2C address, if any.
- Headphone detect switch GPIO, if any.
- Mute, shutdown, or amplifier power-enable GPIOs.
- Whether there is also a speaker path.
- I2S port number and sample-rate limits used by the shipped firmware.

## Recommended Hardware Checks

- Photograph both PCB sides around the headphone jack and nearby ICs. Capture readable package markings.
- Trace headphone jack left/right/sleeve pins to nearby passives and IC pins.
- Check for a small codec/DAC/amplifier near the jack. Candidate markings matter more than package shape.
- Play a known WAV/MP3 and probe likely serial-audio lines with a logic analyzer to identify BCLK, LRCLK, and DOUT.
- Probe for headphone detect state changes while inserting/removing a plug.
- Check whether output level can drive low-impedance headphones or is closer to line-level.

## Recommended Firmware Checks

- In Ghidra, continue function creation around low IROM references near the I2S error strings. Current automated decompilation did not recover reliable functions there.
- Search for `i2s_pin_config_t`-shaped constants in app data/code and cross-reference them to `i2s_set_pin`.
- Search for codec I2C initialization patterns if a codec IC is identified visually.
- Runtime smoke test: put known-good `.wav`, `.mp3`, `.flac`, and `.opus` files on SD, then see which files the stock UI indexes in `/music.list` and plays.

## Porting Impact

CrossPoint can be ported without audio for the first milestone, but the board profile should reserve audio pins once known. If the headphone DAC shares an I2C bus with touch or RTC hardware, that bus must be modeled as a shared board resource instead of claimed by one driver.

