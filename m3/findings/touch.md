# Touch Hardware And Firmware Notes

## High Confidence

The OEM firmware has touch-specific application logic.

Recovered strings from `m3/extracted/app0.bin`:

| String | Evidence |
| --- | --- |
| `touchTask` | FreeRTOS-style task name embedded in the app image. |
| `<Reset custom touch area>` | UI/control string for resetting touch regions. |
| `Touch to exit` | User-facing touch prompt. |
| `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json` | OTA metadata URL points to the Corogoo touch firmware branch. |

Ghidra exports:

- `m3/analysis/touch_string_refs.md`
- `m3/analysis/touch_pointer_refs.md`
- `m3/analysis/touch_decompile.md`

The app is built with Arduino-ESP32/PlatformIO and includes the Arduino `Wire` stack plus ESP-IDF I2C driver code. This proves I2C capability is linked, but generic I2C library strings alone do not prove which device uses it.

## Controller Identity

No reliable plain-text touch-controller identifier was recovered from the Murphy app image. Searches did not find confirmed `GT911`, `GT9`, `Goodix`, `FT6336`, `CST`, `CHSC`, or `XPT2046` strings in `m3/extracted/app0.bin`.

The nearby public Corogoo touch firmware is not byte-identical to the Murphy app, but it is clearly related by strings, project paths, firmware URL, board target, and feature set. A `QGT9` byte sequence appears inside that Corogoo touch firmware. Because the sequence sits in executable-looking Xtensa bytes and the Murphy app has similar accidental ASCII-looking sequences, treat this only as a weak hint toward a GT9xx/Goodix-class controller, not as proof.

Online research originally made Good Display's `FT6336U`-based 3.7-inch `GDEY037T03-FT21` module look like the best external hardware lead. Live probing did find an `0x38` ACK on `SDA=GPIO13` / `SCL=GPIO12`, but its data is static and invalid. The stronger current evidence is now the Murphy OEM binary plus live hardware behavior: function naming/decompilation points at a CHSC6x/Chipsemi-class controller path on address `0x2e`, and the standalone data probe confirms `0x2e` `0xa3`/`0xa5` reads return touch-correlated 5-byte frames.

Current working hypothesis:

| Signal / field | Candidate |
| --- | --- |
| Controller family | CHSC6x / Chipsemi class |
| I2C address | `0x2e` |
| I2C SDA | `GPIO13` |
| I2C SCL | `GPIO12` |
| I2C frequency | 100 kHz |
| Command bytes | `0xa3`, `0xa5`; `0x00` also read for comparison |
| Coordinate read length | 16 bytes |
| Coordinate packing | `raw_x=byte4`, `raw_y=(byte5 << 8) | byte6` |
| INT / wake | `GPIO44`, active-low, confirmed by passive and I2C-gated probing |
| RST / control candidate | `GPIO45`; do not use in normal firmware yet |
| Explicitly excluded | `GPIO48`, confirmed front-light PWM |

Most likely controller families to probe first:

| Family | Typical I2C address | Useful probe notes |
| --- | ---: | --- |
| CHSC6x / Chipsemi class | `0x2e` | Current primary OEM RE hypothesis; command bytes `0xa3`/`0xa5`, 16-byte coordinate read path. |
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

The current standalone touch data probe confirms the OEM-derived CHSC6x-style path:

```text
SDA=GPIO13
SCL=GPIO12
addr=0x2e
commands tried: 0xa3, 0xa5, 0x00
coordinate sample length: 16 bytes
```

Observed from a single tap:

```text
[12524] sample burst: irq-active irq=0 baseline=1
[12528] CHSC_A3 addr=0x2E reg=0xA3 raw=36 02 00 40 88
[12528] CHSC_A5 addr=0x2E reg=0xA5 raw=36 02 00 40 88
[12557] CHSC_A3 addr=0x2E reg=0xA3 raw=36 02 01 80 41
[12991] IRQ GPIO44 0->1 baseline=1
```

Observed from top-left taps with the expanded decoder probe:

```text
CHSC_A3 raw=36 02 00 40 41 raw12=512,65 ... chsc12=208,15/1 dec8=0,65/1
CHSC_A3 raw=36 02 01 80 18 raw12=513,24 ... chsc12=208,5/1 dec8=1,24/1
TOUCH up x=208 y=5 total_dx=0 total_dy=-10 gesture=tap

CHSC_A3 raw=36 02 00 40 18 raw12=512,24 ... chsc12=208,5/1 dec8=0,24/1
TOUCH down x=0 y=24 decoder=chsc8_b2_b4
CHSC_A3 raw=36 02 01 80 29 raw12=513,41 ... chsc12=208,9/1 dec8=1,41/1
TOUCH move x=1 y=41 decoder=chsc8_b2_b4
TOUCH up x=0 y=41 total_dx=0 total_dy=17 gesture=tap
```

Observed from a top-right tap:

```text
CHSC_A3 raw=36 02 00 40 29 ... dec8=0,41/1
TOUCH down x=0 y=41 decoder=chsc8_b2_b4
CHSC_A3 raw=36 02 01 80 C8 ... dec8=1,200/1
TOUCH move x=1 y=200 decoder=chsc8_b2_b4
TOUCH up x=0 y=200 total_dx=0 total_dy=159 gesture=swipe-down
```

Observed from bottom-left and bottom-right taps:

```text
bottom-left:
CHSC_A3 raw=36 02 01 80 17 ... dec8=1,23/1
TOUCH move x=1 y=23 decoder=chsc8_b2_b4
TOUCH up x=0 y=23 total_dx=0 total_dy=-177 gesture=swipe-up

bottom-right:
CHSC_A3 raw=36 02 01 80 DF ... dec8=1,223/1
TOUCH move x=1 y=223 decoder=chsc8_b2_b4
TOUCH up x=0 y=223 total_dx=0 total_dy=200 gesture=swipe-down
```

Observed from widened 16-byte reads, using the stable frames after the first stale sample:

```text
top-left:     raw=36 02 01 80 0E 00 2E 0D 10 ...  byte4=14  byte5:6=0x002e
top-right:    raw=36 02 01 80 DC 00 18 0D 10 ...  byte4=220 byte5:6=0x0018
bottom-left:  raw=36 02 01 80 15 01 84 0D 10 ...  byte4=21  byte5:6=0x0184
bottom-right: raw=36 02 01 80 DC 01 88 0D 10 ...  byte4=220 byte5:6=0x0188
center:       raw=36 02 01 80 84 00 D3 0D 10 ...  byte4=132 byte5:6=0x00d3
```

Interpretation:

- `GPIO44` is active-low: idle baseline was `1`, touch asserted `0`, and it returned to `1`.
- `0x2e` is a real device on the `GPIO13/GPIO12` I2C bus.
- `0xa3` and `0xa5` currently return the same frame and change during touch.
- Reading the frame appears to clear/release the IRQ quickly, so normal firmware should treat the IRQ as an event trigger, not as a level that must stay active during the whole read.
- The earlier `raw12` decode is now wrong for screen coordinates: physical top-left taps decoded to approximately `x=208` under `raw12`, which is near the horizontal center of the 416-pixel landscape surface.
- The compact byte decode is touch-correlated and tracks left-vs-right motion:

```text
x = byte2
y = byte4
```

However, the first 5 bytes of `0xa3`/`0xa5` do not distinguish top-vs-bottom: both top-left and bottom-left produce `byte4` near the low end, and both top-right and bottom-right produce `byte4` near the high end, while `byte2` stays near `0/1`.

The widened 16-byte probe found the missing axis. Current calibrated coordinate decode:

```text
raw_x = byte4
raw_y = (byte5 << 8) | byte6

raw_x ~= 24..224 -> screen x 0..415
raw_y ~= 24..392 -> screen y 0..239
```

The first sample after IRQ often contains the previous touch position and should be ignored for coordinate events. `murphy_touch_data_probe_v7_b4_b56_calibrated` ignores that first stale sample, reads 16 bytes from `0xa3`, `0xa5`, and `0x00`, and prints changed samples only. Normal firmware should reject all-zero coordinate frames and all-`0xff` junk frames.

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

- Add `HalTouch` or equivalent behind `BoardConfig::hasTouch`; the current SDK integration exposes calibrated touch points through `InputManager`.
- Model touch as `TouchConfig` in `BoardConfig`, with controller type, pins, address, calibration bounds, IRQ polarity, and whether a touch should synthesize a logical confirm button. Murphy should keep touch and GPIO buttons separate, so `synthesizeConfirmButton=false`.
- Start with the OEM-derived CHSC6x path on `SDA=GPIO13`, `SCL=GPIO12`, address `0x2e`, commands `0xa3`/`0xa5`.
- Use `GPIO44` as an active-low interrupt/wake input. Treat it as an event gate because I2C reads may clear the IRQ.
- Keep the stale `0x38` FT-style ACK visible in logs, but do not use it for touch navigation unless it produces sane, tap-correlated frames.
- Publish touch events as screen coordinates using `byte4` plus big-endian `byte5:byte6` after an IRQ-gated delayed read.
- In CrossPoint, keep the SDK at the calibrated point/event layer and map touch in UI code: full left/right reader sides page backward/forward, top-left reader tap goes back, center hold opens the reader menu, and list rows/menu rows activate directly.
- Add input clearing plus a short transition suppression window when activities enter. Without that, a touch or button release used to leave the reader can be consumed by the Home screen and reopen the recent book immediately.
- Add a touch-target registry or equivalent UI helper layer so existing menu rows/buttons can expose rectangles and map taps to the same actions as button presses.

Keep CrossPoint fully usable with GPIO buttons. Touch should be additive.

## Next Hardware Tests

1. Keep active touch polling IRQ-gated and bounded; prior ungated polling caused boot loops and made recovery difficult.
2. Use `GPIO44` as the confirmed active-low touch IRQ to gate future probes and production reads.
3. Use `murphy_touch_data_probe_v7_b4_b56_calibrated` if recalibration is needed; current corner and center taps land near expected 416x240 screen coordinates.
4. Verify CrossPoint TXT and EPUB readers independently: EPUB already has an EPUB-specific menu, while TXT needs its own menu path because it does not share the EPUB menu actions.
5. Keep future I2C scanners bounded and exclude `GPIO19`, `GPIO20`, `GPIO45`, `GPIO46`, and `GPIO48` unless deliberately testing those pins in a recovery-safe image.
6. Continue refining UI touch-target rectangles in CrossPoint without changing the low-level pin/controller path.
