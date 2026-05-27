# Display Driver and Refresh Path

Scope: Murphy M3 only. UC8253 panel, OEM-derived bit-banged bus. For the panel ID, FPC markings, and confirmed GPIO bus, see [Display Hardware](display_hardware.md).

## OEM Driver Dataflow

### Byte Writer (proves MOSI/SCK)

`FUN_42038648`:

```c
FUN_42038648(byte value) {
  repeat 8 times:
    digitalWrite(4, 0);
    digitalWrite(3, value & 0x80);
    digitalWrite(4, 1);
    value <<= 1;
  digitalWrite(4, 0);
}
```

`GPIO3 = MOSI`, `GPIO4 = SCK`.

### Command/Data Wrapper (proves CS/DC)

`FUN_4203868c`:

```text
GPIO6 low      ; DC -> command
GPIO5 low      ; CS asserted
write byte
GPIO5 high
GPIO6 high
```

`GPIO5 = CS`, `GPIO6 = DC`. Higher-level command/data helpers confirm the same object fields (`0x10` = CS, `0x12` = DC).

### Constructor Chain (proves RST/BUSY and geometry)

Display constructor wrapper is `FUN_42038810`, calling `FUN_42038280`:

```c
FUN_42038280(object, cs, dc, rst, busy, 0, ..., 0xf0, 0x1a0, ...);
```

`0xf0 x 0x1a0` = 240 x 416. The caller around `0x42022618` supplies `CS=5, DC=6, RST=7, BUSY=8` via Xtensa windowed-call arg registers `a11..a14`.

Constructor stores into object fields:

| Object offset | Arg | Role |
| ---: | ---: | --- |
| `0x10` | `5` | CS |
| `0x12` | `6` | DC |
| `0x14` | `7` | RST |
| `0x16` | `8` | BUSY |

Reset helper `FUN_420382fc` pulses the field at `0x14`. Important correction: the public CrowPanel docs list GPIO7 as a display power enable, but the Murphy OEM driver uses GPIO7 as reset.

### BUSY Polarity

Init at `FUN_420389ec` waits while BUSY reads zero:

```c
while (digitalRead(*(uint16_t *)(object + 0x16)) == 0) {
  delay(10);
}
```

BUSY ready polarity is high.

## OEM Init Sequence

Main init: `FUN_420389ec(object, mode)`. Default mode (`mode == 0`):

```text
reset
wait BUSY ready-high

0x01: 03 10 3F 3B 0D
0x06: D7 D7 1F
0x04                ; power on, wait
0x00: FF
0x30: 09
0x61: F0 01 A0
0x82: 0F
0x50: 97
```

Alternate branch (`mode != 0`): fifth byte of `0x01` and the `0x82` value change to `00`. The working probe uses the default `mode == 0` sequence.

Alternate `0x01` payload, also seen in OEM:

```text
0x01: 03 10 3F 3F 03
```

(Extracted at `0x3c236ca3`.)

## Frame Write and Refresh

OEM full-screen path: `FUN_42038cac`.

```text
FUN_42038b60(object)       ; LUT/table load (0x20..0x24)
write image to 0x10        ; "old" plane
write image to 0x13        ; "new" plane
FUN_4203885c(...)          ; commit/update wrapper
```

Important: the OEM plane-write helper is called **twice with the same source buffer** — once for `0x10`, once for `0x13`. There is no old-vs-new differential write in the default refresh path. The default LUTs are destination-only: with `(old == new)` everywhere, only LUTWW/LUTBB fire — the long ghost-clearing waveforms that fully drive each pixel to its target value. LUTBW/LUTWB exist for the alternate `0x17/0xA5` partial-refresh path and only do a short kick. Using them on a differential leaves pixels half-flipped (verified empirically).

Refresh and power helpers:

| Function | Meaning | Key commands |
| --- | --- | --- |
| `FUN_420389cc` | Power on | `0x04`, waits/logs `_PowerOn` |
| `FUN_42038f24` | Power off | `0x02`, waits/logs `_PowerOff` |
| `FUN_42038f74` | Fast update | default mode: `0x12`; else `0x17/0xA5`, waits `_Update_Fast` |
| `FUN_42038fa4` | Partial update | `0x12` or `0x17/0xA5`, waits `_Update_Part` |

Related app strings: `_PowerOn`, `_PowerOff`, `_Update_Fast`, `_Update_Part`, `displayOtaTask`, `Not full refresh`, `LUT_DEL`, `LUT_ADD`, `LUT_FULL_ADD`, `/lut.bin`, `/lut_full.bin`. `displayOtaTask` points to `FUN_4201482c` (display OTA/upload progress, not the panel bus itself).

## LUT Tables

LUTs were extracted from `analysis/segments/app0_seg0_3c190020.bin`. `FUN_42038b60` loads them with commands `0x20..0x24` before each frame write.

### Default Set (5 x 42 bytes)

```cpp
static constexpr uint8_t MURPHY_LUT_20_DEFAULT[] = {
    0x01, 0x08, 0x08, 0x08, 0x08, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x08,
    0x08, 0x08, 0x08, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_21_DEFAULT[] = {
    0x01, 0x48, 0x48, 0x48, 0x48, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x88,
    0x88, 0x88, 0x88, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_22_DEFAULT[] = {
    0x01, 0x48, 0x48, 0x48, 0x48, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x88,
    0x88, 0x88, 0x88, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_23_DEFAULT[] = {
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x48,
    0x48, 0x48, 0x48, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_24_DEFAULT[] = {
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x48,
    0x48, 0x48, 0x48, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
```

Extracted addresses:

| Command | Address | Length |
| ---: | ---: | ---: |
| `0x20` | `0x3c236f8c` | `0x2a` |
| `0x21` | `0x3c236f62` | `0x2a` |
| `0x22` | `0x3c236f38` | `0x2a` |
| `0x23` | `0x3c236f0e` | `0x2a` |
| `0x24` | `0x3c236ee4` | `0x2a` |

### Alternate Set (mixed 56/42 bytes, used with `0x17/0xA5` refresh)

```cpp
static constexpr uint8_t MURPHY_LUT_20_ALT[] = {
    0x01, 0x0F, 0x0F, 0x0F, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_21_ALT[] = {
    0x01, 0x4F, 0x8F, 0x0F, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_22_ALT_A[] = {
    0x01, 0x4F, 0x8F, 0x4F, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_23_ALT_B[] = {
    0x01, 0x0F, 0x8F, 0x0F, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static constexpr uint8_t MURPHY_LUT_24_ALT[] = {
    0x01, 0x0F, 0x8F, 0x4F, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00,
};
```

Extracted addresses:

| Table | Address | Length |
| --- | ---: | ---: |
| `MURPHY_LUT_20_ALT` | `0x3c23706c` | `0x38` |
| `MURPHY_LUT_21_ALT` | `0x3c237042` | `0x2a` |
| `MURPHY_LUT_22_ALT_A` | `0x3c236fe0` | `0x2a` |
| `MURPHY_LUT_23_ALT_B` | `0x3c23700a` | `0x38` |
| `MURPHY_LUT_24_ALT` | `0x3c236fb6` | `0x2a` |

The current SDK port loads the default `0x20..0x24` set before Murphy full-frame writes. The alternate set is documented but not active because it belongs with the alternate init/refresh mode.

### Third Region Is NOT a Grayscale LUT (corrected)

An earlier pass identified a third LUT-shaped region at `0x3c236d24..0x3c236df5` (five 42-byte tables at stride `0x2a`) and hypothesised a factory grayscale LUT set. Bench-testing disproved this.

The pointer table at `0x420022e0..0x420022f4` is read by code around `0x420dee2f` (`l32r a8, 0x420022e0; callx8 a8`) — the address `0x3c236df6` stored there is being **called as a function**. ESP32-S3 maps the same flash content through both the DROM (`0x3c…`) and IROM (`0x42…`) windows, so the "LUT-shaped" tables at `0x3c236d24+` are actually code or data structs accessed via the DROM alias of an executable region. The bytes happen to satisfy UC8253 LUT format closely enough that the controller accepts them, but with non-design drive intensities.

Conclusion: **the factory firmware does not contain a grayscale LUT set.** The OEM never validated grayscale on this panel. Anyone pursuing grayscale must hand-tune from scratch — see below.

## Practical Image Writer

```cpp
resetPanel();
initUc8253OemSimple();
loadDefaultLuts();  // 0x20..0x24 with MURPHY_LUT_*_DEFAULT

writeCommand(0x10);
writePlane(currentFrame, 12480);  // same buffer to both planes

writeCommand(0x13);
writePlane(currentFrame, 12480);

writeCommand(0x12);
waitBusyReadyHigh("display refresh", 12000);

writeCommand(0x02);
waitBusyReadyHigh("power off", 5000);
```

Frame size: `240 x 416 / 8 = 12480` bytes per 1-bit plane. If polarity is inverted on the panel, invert every byte of `currentFrame` before sending. The visible "full inversion flash" before the image settles is intentional in these LUTs — phases drive positive then negative before holding, which clears ghosting.

## Grayscale Bench-Tuning Findings

Bench-tuning probe: `crosspoint-reader-main/src/murphy_grayscale_tuning_probe.cpp`. Writes a known `(DTM1_bit, DTM2_bit)` pattern across four horizontal stripes; LUT bytes and power voltages are edited at the top of the file.

### Voltage Codes (via the FAST B/W LUT path)

- `0x88` drives toward white (VSL polarity)
- `0x48` drives toward black (VSH polarity)
- `0x00` holds (no drive)
- Voltage codes appear in sub-phase byte positions 1-4 of each 7-byte phase block.

### Panel Behaviors

**Asymmetric rails.** VSH drives much harder per frame than VSL on this panel. Any pattern with 2+ VSH (`0x48`) sub-phases collapses to solid black, even paired with 2 VSL sub-phases. Pure VSL drive (`88 88 88 88`) only reaches a light gray ("muddy white") at default voltages.

**LUT slot asymmetry.** Same byte pattern produces different intensities depending on which slot (`0x21`/`0x22`/`0x23`/`0x24`) it's loaded into. Verified: `88 88 88 88` lands at light gray in `LUT_22` but clean white in `LUT_23`. Byte tuning is per-slot.

**DC bias drift.** Pure single-direction multi-phase LUTs drive pixels to target initially, then drift back toward neutral gray over seconds. VSH compensation pulls saturated whites significantly back toward black. The OEM kick+settle+drive structure is DC-balanced and stable, but on the asymmetric-rail Murphy panel only the structure used in `LUT_24` lands cleanly at white.

### Achievable Intensities

| Pattern (single-phase) | Result on Murphy |
| --- | --- |
| Any with 2+ `0x48` sub-phases | **Black** |
| `01 88 88 88 48 01 01` (3 white + 1 black) | **Dark gray** |
| `01 88 88 88 88 01 01` (pure white drive) | **Light gray** in `LUT_22`; **clean white** in `LUT_23` |
| OEM-default 3-phase `48/02/88` pattern in `LUT_24` | **Stable clean white** |

Three cleanly-distinguishable levels. No usable intermediate between dark gray and light gray.

### Final Hand-Tuned Voltages and LUTs

```text
BOOSTER_SETTING = {0xD7, 0xDF, 0x1F}     // one notch above OEM VSL byte
VCOM_DC_SETTING = 0x03                   // alt-OEM VCOM (vs default 0x0F)
```

```cpp
// LUT_21 (DTM1=1, DTM2=1) -> BLACK (DC-balanced)
constexpr uint8_t LUT_21[] = {
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01,  // negative kick
    0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01,  // settle
    0x01, 0x48, 0x48, 0x48, 0x48, 0x01, 0x01,  // positive drive -> black
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_22 (DTM1=0, DTM2=1) -> LIGHT GRAY
constexpr uint8_t LUT_22[] = {
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_23 (DTM1=1, DTM2=0) -> DARK GRAY
constexpr uint8_t LUT_23[] = {
    0x01, 0x88, 0x88, 0x88, 0x48, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_24 (DTM1=0, DTM2=0) -> WHITE (DC-balanced)
constexpr uint8_t LUT_24[] = {
    0x01, 0x48, 0x48, 0x48, 0x48, 0x01, 0x01,  // positive kick
    0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01,  // settle
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01,  // negative drive -> white
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_20 (VCOM) — 3 active phases tracking the longest drive
constexpr uint8_t LUT_20[] = {
    0x01, 0x08, 0x08, 0x08, 0x08, 0x01, 0x01,
    0x01, 0x08, 0x08, 0x08, 0x08, 0x01, 0x01,
    0x01, 0x08, 0x08, 0x08, 0x08, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
```

### Renderer Encoding Caveat

The renderer encodes 2-bpp values as `LSB→DTM1, MSB→DTM2`:

| Renderer value | (LSB, MSB) | Fires | Intensity with recipe above |
| ---: | --- | --- | --- |
| 0 (white) | (0,0) | LUT_24 | **white** ✓ |
| 1 (light mid) | (1,0) | LUT_23 | **dark gray** ⚠ inverted |
| 2 (dark mid) | (0,1) | LUT_22 | **light gray** ⚠ inverted |
| 3 (black) | (1,1) | LUT_21 | **black** ✓ |

Mid-tones are inverted relative to renderer expectation. Fix: swap which plane goes to which DTM at the ingest layer (`LSB→DTM2, MSB→DTM1`). One-line change in `copyGrayscaleBuffers` / `copyGrayscale*Buffers`.

### Open Levers (Not Yet Swept)

- `VCOM_DATA_INTERVAL` (cmd `0x50`, currently `0x97`) — border behavior may affect post-drive idle drift.
- Booster bytes 0 (VSH ramp) and 2 (timing) — only byte 1 (VSL ramp) was swept.
- Multi-pass refresh: send gray LUT twice in a row to accumulate drive.
- `byte 0` slot of a phase block — earlier assumed it was repeat-count; `0x04` there broke drive. Could be TP source select.

## Current Shipping State: AA Disabled

**Murphy ships AA-disabled.** B/W FAST path with periodic ghost-clear refresh — full black text on white, no anti-aliasing pipeline.

Gated at the application layer via `DeviceProfile::supportsGrayscaleAntiAlias` (`false` for Murphy, `true` for X3/X4) and a `ReaderUtils::effectiveAntiAlias()` helper that combines the user's text-AA setting with device capability. The renderer's two-pass AA path (`copyGrayscaleLsbBuffers` + `copyGrayscaleMsbBuffers` + `displayGrayBuffer`) never executes on Murphy.

### Why AA Was Ripped Out

1. **Per-pixel pixelation.** The renderer produces detailed per-pixel `(LSB, MSB)` patterns at glyph edges. With the hand-tuned 4-LUT set, each `(DTM1, DTM2)` combination produces a distinctly different intensity — adjacent pixels look like a noisy quilt rather than smooth AA.
2. **Multi-refresh degenerate frames.** The two-pass AA flow calls `displayGrayBuffer` multiple times per page render, some with nearly-all-zero or all-set planes. The panel cycles through "correct text → fully inverted → all white" as each pass commits.
3. **`cleanupGrayscaleBuffers` fires SSD1677 commands.** The default SDK cleanup path writes to SSD1677 RED-RAM (`writeRamBuffer(CMD_WRITE_RAM_RED, ...)`). UC8253 doesn't have that register; commands corrupt controller state.
4. **Asymmetric VSH/VSL rails.** Hardware property — no LUT trick gets past it cleanly.

The probe *did* produce four clearly distinct horizontal stripes with hand-tuned LUTs and bumped voltages, proving the panel can technically do 4-level grayscale. But "uniform regions hit four levels" ≠ "text rendered with per-pixel AA looks good" on this hardware.

### Brief Soft-Text Experiment (Removed)

For a short window we shipped a "soft text" mode: kept B/W FAST intact but swapped `LUT_24` (dark-pixel slot in same-buffer scheme) from full black (`0x48 0x48 0x48 0x48`) to dark gray (`0x88 0x88 0x88 0x48`), gated on `SETTINGS.textAntiAliasing`. Worked — uniform dark-gray text on white, no AA pipeline, no pixelation. But the visual benefit was marginal vs. crisp black and not worth the extra SDK surface. Removed.

### Future Work

- 4-LUT recipe with `BOOSTER = {0xD7, 0xDF, 0x1F}` and `VCOM_DC = 0x03` is the best known stable state for 4 distinct uniform intensities.
- Real per-pixel AA would need a custom 1-pass grayscale refresh (bypassing the two-pass `copyGrayscale*` + `displayGrayBuffer` path) plus a renderer-side change to emit per-pixel intensities in a single buffer.
- The `0x17, 0xA5` partial-refresh trigger path is worth investigating for region-only updates without ghost-clear cost (orthogonal to grayscale).

## Related Artifacts

- `analysis/display_driver_core_decompile_20260526.md`
- `analysis/display_function_refs_20260526.md`
- `analysis/display_high_level_refs_20260526.md`
- `analysis/segments/app0_seg0_3c190020.bin` (LUT source)
