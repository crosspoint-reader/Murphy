# Touch Hardware And Firmware Notes

## High Confidence

The OEM firmware has touch-specific application logic.

Recovered strings from `analysis/extracted/app0.bin`:

| String | Evidence |
| --- | --- |
| `touchTask` | FreeRTOS-style task name embedded in the app image. |
| `<Reset custom touch area>` | UI/control string for resetting touch regions. |
| `Touch to exit` | User-facing touch prompt. |
| `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json` | OTA metadata URL points to the Corogoo touch firmware branch. |

Ghidra exports:

- `analysis/touch_string_refs.md`
- `analysis/touch_pointer_refs.md`
- `analysis/touch_decompile.md`

The app is built with Arduino-ESP32/PlatformIO and includes the Arduino `Wire` stack plus ESP-IDF I2C driver code. This proves I2C capability is linked, but generic I2C library strings alone do not prove which device uses it.

## Controller Identity

No reliable plain-text touch-controller identifier was recovered from the Murphy app image. Searches did not find confirmed `GT911`, `GT9`, `Goodix`, `FT6336`, `CST`, `CHSC`, or `XPT2046` strings in `analysis/extracted/app0.bin`.

The nearby public Corogoo touch firmware is not byte-identical to the Murphy app, but it is clearly related by strings, project paths, firmware URL, board target, and feature set. A `QGT9` byte sequence appears inside that Corogoo touch firmware. Because the sequence sits in executable-looking Xtensa bytes and the Murphy app has similar accidental ASCII-looking sequences, treat this only as a weak hint toward a GT9xx/Goodix-class controller, not as proof.

Online research found a stronger external hardware lead: Good Display's 3.7-inch `GDEY037T03-T02` and `GDEY037T03-FT21` panels match the Murphy/CrowPanel resolution and UC8253 display controller family, and Good Display lists the touch controller as `FT6336U` over I2C. The `FT21` variant also includes front light, matching the HamGeek M3 product listing's 10-level backlight/front-light claim. This does not prove the Murphy panel is that exact Good Display module, but it makes FocalTech/FT6x36 the first driver family to try.

Most likely controller families to probe first:

| Family | Typical I2C address | Useful probe notes |
| --- | ---: | --- |
| FocalTech FT6x36 class | `0x38` | Common touch-data registers start near `0x02`. |
| Goodix GT9xx / GT911 class | `0x5d` or `0x14` | Common registers include touch status around `0x814e` and coordinate data after that. |
| CST816/CST8xx class | `0x15` | Common on small capacitive panels; simple point/gesture registers. |
| CHSC6x class | `0x2e` | Seen on low-cost capacitive modules. |

Good Display touch-panel pin order for the related 3.7-inch touch panel:

| Pin | Signal |
| ---: | --- |
| 1 | GND |
| 2 | VDD |
| 3 | RST |
| 4 | INT |
| 5 | SDA |
| 6 | SCL |

## Pin/Routing Evidence

The public Elecrow CrowPanel 3.7-inch schematic defines display FPC touch pins:

- U4 pad 6: `TSCL`
- U4 pad 7: `TSDA`

However, the matching public `.brd` file does not contain routed `contactref` entries for U4 pads 6 or 7. The public schematic/board package therefore does not give a usable ESP32-S3 GPIO mapping for touch.

Known routed display pins from the public CrowPanel reference are separate SPI display pins:

| Signal | ESP32-S3 GPIO |
| --- | ---: |
| EPD `SDA` / MOSI | GPIO11 |
| EPD `SCL` / CLK | GPIO12 |
| EPD `BUSY` | GPIO48 in public CrowPanel files only; invalid for Murphy because GPIO48 is confirmed front-light PWM |
| EPD `RES` | GPIO47 |
| EPD `D/C` | GPIO46 |
| EPD `CS` | GPIO45 |

Do not assume `TSCL`/`TSDA` share GPIO11/GPIO12. Those are the e-paper SPI pins in the reference board files, not proven touch I2C pins. Also do not use GPIO48 for touch probing on Murphy; custom firmware confirms it is the front-light PWM pin and it can create misleading all-address I2C ACK results when pulled or driven.

## Custom Probe Results

Several non-invasive custom probes have now run from `/Users/jmitch/GitHub/crosspoint-reader-main`.

Broad I2C pair sweeping produced many false positives until stuck/coupled-line detection was added. The most important artifact was GPIO48: sweeps involving `SDA=48` produced all-address ACK storms and also affected the front light. GPIO48 is now confirmed as front-light control, not touch I2C.

The only plausible touch-looking candidate found so far is:

| Candidate | Result |
| --- | --- |
| `SDA=13`, `SCL=12`, address `0x38` | ACKs repeatably, but does not produce valid touch data. |

Reads from that candidate show fixed, invalid FT6x36-style frames:

```text
0x00: 18 7E EE A6 1A 7C FF FF ...
0x02: 18 30 80 A6 1A 7C FF FF ...
0xA0: 10 1C BC BA 1A 7C FF FF ...
```

Single taps and drags did not change those bytes. Decoding them as FT6x36 data produces impossible point counts and coordinates, so `SDA=13/SCL=12 @ 0x38` should be treated as an unknown responder or false candidate, not as confirmed touch.

Passive GPIO input sampling also found no tap-correlated IRQ line. A later pull-up probe turned the front light on because it touched GPIO48, reinforcing that GPIO48 must be excluded from future touch/IRQ sweeps.

## OEM Logic Recovered So Far

The firmware appears to model touch as regions/actions rather than only raw coordinates. Evidence:

- `touchTask` suggests a dedicated polling or event task.
- `<Reset custom touch area>` suggests configurable or runtime-defined touch hitboxes.
- `Touch to exit` suggests a modal/fullscreen prompt where any touch exits.

The current Ghidra project is good enough to find strings and some xrefs, but decompilation around the touch strings is still noisy because many references are through literal pools and generated tables. `FindPointerRefs.java` was added to chase those pointer locations.

## Porting Impact

For `community-sdk`, the right shape is a board-level touch abstraction rather than wiring touch directly into menu code:

- Add `HalTouch` or equivalent behind `BoardConfig::hasTouch`.
- Start with an I2C scan/probe path on candidate buses, explicitly excluding GPIO48.
- Publish touch events as screen coordinates plus simple gestures.
- Add a touch-target registry in the UI layer so existing menu rows/buttons can expose rectangles and map taps to the same actions as button presses.

Until pins and controller are verified, keep CrossPoint fully usable with GPIO buttons. Touch should be additive.

## Next Hardware Tests

1. Continue OEM/static RE for touch GPIO constants and initialization calls.
2. Run future I2C scanners only on bounded candidate pairs and exclude GPIO48.
3. Probe common touch addresses: `0x38`, `0x15`, `0x5d`, `0x14`, `0x2e`.
4. If a device ACKs, require tap-correlated register changes before treating it as touch.
5. Once pins/address are known, add them to `community-sdk/libs/hardware/BoardConfig/include/BoardConfig.h`.
