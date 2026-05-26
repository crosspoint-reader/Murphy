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

Online research originally made Good Display's `FT6336U`-based 3.7-inch `GDEY037T03-FT21` module look like the best external hardware lead. Live probing did find an `0x38` ACK on `SDA=GPIO13` / `SCL=GPIO12`, but its data is static and invalid. The stronger current evidence is now the Murphy OEM binary itself: function naming/decompilation points at a CHSC6x/Chipsemi-class controller path on address `0x2e`, with command bytes `0xa3`/`0xa5` and a compact 5-byte sample read path.

Current working hypothesis:

| Signal / field | Candidate |
| --- | --- |
| Controller family | CHSC6x / Chipsemi class |
| I2C address | `0x2e` |
| I2C SDA | `GPIO13` |
| I2C SCL | `GPIO12` |
| I2C frequency | 100 kHz |
| Command bytes | `0xa3`, `0xa5` |
| Sample length | 5 bytes |
| INT / wake | `GPIO44`, confirmed by passive GPIO probing |
| RST / control candidate | `GPIO45` |
| Explicitly excluded | `GPIO48`, confirmed front-light PWM |

Most likely controller families to probe first:

| Family | Typical I2C address | Useful probe notes |
| --- | ---: | --- |
| CHSC6x / Chipsemi class | `0x2e` | Current primary OEM RE hypothesis; command bytes `0xa3`/`0xa5`, 5-byte sample path. |
| FocalTech FT6x36 class | `0x38` | External panel docs suggested this, but current Murphy reads are static/invalid. Treat as false/stale unless future tap-correlated data appears. |
| Goodix GT9xx / GT911 class | `0x5d` or `0x14` | Common registers include touch status around `0x814e` and coordinate data after that. |
| CST816/CST8xx class | `0x15` | Common on small capacitive panels; simple point/gesture registers. |

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

The earlier broad probes found one repeatable but currently rejected FT-style candidate:

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

The current CrossPoint/community-sdk test path has been updated to try the OEM-derived CHSC6x-style reads first:

```text
SDA=GPIO13
SCL=GPIO12
addr=0x2e
commands tried: 0xa3, 0xa5, 0x00
sample length: 5 bytes
```

Expected serial log shape from the updated SDK:

```text
[TOUCH] Murphy touch init chsc=0x2E ack=<0/1> read=<0/1> cmd=0x<A3/A5/00> old_ft=0x38 ack=<0/1> sda=13 scl=12 int=44 rst=45
[TOUCH] chsc6x cmd=0x<A3/A5/00> raw=<5 bytes> pressed=<0/1> x=<n> y=<n> int=<0/1>
```

A useful hardware confirmation requires tap-correlated changes in the 5-byte CHSC frame. An ACK alone is not enough.

Passive GPIO input sampling now confirms `GPIO44` changes with touch activity. Earlier broad probes failed because they mixed too many candidate pins and included risky or misleading pins. GPIO48 in particular turned on the front light when pulled, reinforcing that GPIO48 must be excluded from future touch/IRQ sweeps.

## OEM Logic Recovered So Far

The firmware appears to model touch as regions/actions rather than only raw coordinates. Evidence:

- `touchTask` suggests a dedicated polling or event task.
- `<Reset custom touch area>` suggests configurable or runtime-defined touch hitboxes.
- `Touch to exit` suggests a modal/fullscreen prompt where any touch exits.

The current Ghidra project now has a Murphy-specific `MurphyTouch` function tag and named/commented touch cluster. Important named functions include:

| Address | Name |
| --- | --- |
| `0x42016f20` | `InitializeTouchTask` |
| `0x42016f00` | `ProcessTouchTaskLoop` |
| `0x42016e9c` | `PollTouchSampleAndQueueEvent` |
| `0x42016f7c` | `DispatchTouchEvent` |
| `0x42016dfc` | `SetTouchEventCoords` |
| `0x420636f0` | `InitializeTouchDeviceStruct` |
| `0x42063708` | `InitializeTouchControllerPins` |
| `0x4206373c` | `ReadTouchRegisterCandidate` |
| `0x42063784` | `ReadTouchSampleCandidate` |
| `0x42063804` | `ReadTouchCommandResponseCandidate` |
| `0x4206384c` | `ReadTouchCommandA5Candidate` |
| `0x4206385c` | `ReadTouchCommandA3Candidate` |

## Porting Impact

For `community-sdk`, the right shape is a board-level touch abstraction rather than wiring touch directly into menu code:

- Add `HalTouch` or equivalent behind `BoardConfig::hasTouch`.
- Start with the OEM-derived CHSC6x path on `SDA=GPIO13`, `SCL=GPIO12`, address `0x2e`, commands `0xa3`/`0xa5`.
- Use `GPIO44` as the interrupt/wake input once the controller read path is stable.
- Keep the stale `0x38` FT-style ACK visible in logs, but do not use it for touch navigation unless it produces sane, tap-correlated frames.
- Publish touch events as screen coordinates plus simple gestures.
- Add a touch-target registry in the UI layer so existing menu rows/buttons can expose rectangles and map taps to the same actions as button presses.

Until pins and controller are verified, keep CrossPoint fully usable with GPIO buttons. Touch should be additive.

## Next Hardware Tests

1. Keep active touch polling out of normal CrossPoint builds until the controller read path is stable; prior active polling caused boot loops and made recovery difficult.
2. Use `GPIO44` as the confirmed touch IRQ to gate future probes.
3. Tap/drag while watching for tap-correlated 5-byte CHSC frame changes on the OEM-derived `SDA=GPIO13`, `SCL=GPIO12`, `0x2e` path.
4. If `0x2e` still does not ACK/read, re-check the reset/control sequence on `GPIO45`.
5. Keep future I2C scanners bounded and exclude `GPIO19`, `GPIO20`, `GPIO45`, `GPIO46`, and `GPIO48` unless deliberately testing those pins in a recovery-safe image.
6. Once tap-correlated CHSC frames are confirmed, promote the path into a board-level touch abstraction and map touch targets to CrossPoint actions.
