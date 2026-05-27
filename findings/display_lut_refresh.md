# Murphy M3 Display LUT And Refresh Path

Created: 2026-05-26

Scope: Murphy M3 only. Do not mix these UC8253 tables or GPIO assumptions with Xteink X3/X4 firmware.

## Status

The display can now be written from custom firmware. The working probe is:

- `murphy_epd_gpio3_8_probe.cpp`
- original external copy: `/Users/patryk/Downloads/murphy_epd_gpio3_8_probe.cpp`

Confirmed working low-level route:

```text
MOSI=3
SCK=4
CS=5
DC=6
RST=7
BUSY=8 ready-high
Width=240
Height=416
Frame bytes per 1-bit plane=12480
```

The next useful step is to turn the probe into a real image writer:

1. Convert an image to a 1-bit `240 x 416` packed framebuffer.
2. Send the **current** plane with command `0x10`.
3. Send the **current** plane again with command `0x13` (see note below).
4. Refresh with command `0x12`.
5. Wait for BUSY ready-high.
6. Power off with command `0x02`.

Important: the OEM frame writer `FUN_42038cac` (`analysis/display_function_refs_20260526.md:497-513`) calls the plane-write helper twice with the **same source buffer** — once for `0x10`, once for `0x13`. There is no old-vs-new differential write in the default refresh path. The default LUTs are destination-only: with `(old=new)` for every pixel, only LUTWW/LUTBB fire, and those are the long ghost-clearing waveforms that fully drive each pixel to its target value. The LUTBW/LUTWB tables exist for the alternate `0x17/0xA5` partial-refresh path and only do a short kick — using them on a "changed pixel" differential leaves pixels half-flipped (verified empirically: a prev-vs-new write flashes but doesn't latch, and a second refresh "completes" the previous frame).

LUTs are not required for the first real image test because the probe already writes black and white. The OEM LUT tables below are the next lever for cleaner full refresh, partial refresh, and possibly faster updates.

## Evidence

The OEM display function `FUN_42038b60` loads waveform/data tables with commands `0x20..0x24`.

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:396-438`

The function is called before OEM frame writes in `FUN_42038cac`, which writes command `0x10`, then command `0x13`, then refreshes elsewhere.

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:447-462`
- `analysis/display_driver_core_decompile_20260526.md:502-535`

The bytes below were extracted from:

- `analysis/segments/app0_seg0_3c190020.bin`
- segment base `0x3c190020`

## OEM Init Branches

The working probe currently uses the simple OEM init branch:

```text
0x01: 03 10 3F 3B 0D
0x06: D7 D7 1F
0x04: power on, wait ready-high
0x00: FF
0x30: 09
0x61: F0 01 A0
0x82: 0F
0x50: 97
```

The alternate OEM branch uses command `0x01` with these 5 bytes:

```cpp
static constexpr uint8_t MURPHY_OEM_ALT_01[] = {
    0x03, 0x10, 0x3F, 0x3F, 0x03,
};
```

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:372-389`
- extracted address `0x3c236ca3`

## Default LUT Set

OEM default branch in `FUN_42038b60` sends:

```text
0x20 <- LUT_20_DEFAULT, 42 bytes
0x21 <- LUT_21_DEFAULT, 42 bytes
0x22 <- LUT_22_DEFAULT, 42 bytes
0x23 <- LUT_23_DEFAULT, 42 bytes
0x24 <- LUT_24_DEFAULT, 42 bytes
```

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

## Alternate LUT Set

The alternate OEM branch sends:

```text
0x20 <- LUT_20_ALT, 56 bytes
0x21 <- LUT_21_ALT, 42 bytes
0x24 <- LUT_24_ALT, 42 bytes
0x22/0x23 <- LUT_22_ALT_A and LUT_23_ALT_B, order depends on one RAM flag
```

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

## Third Byte Region — NOT a Factory Grayscale LUT (corrected)

An earlier pass through this analysis identified a third LUT-shaped region at
`0x3c236d24..0x3c236df5` (five 42-byte tables at stride `0x2a`) and
hypothesised it was a factory grayscale LUT set. Bench-testing on hardware
disproved this. The bytes do produce visible (if washed-out) grayscale when
sent to the controller, but that's coincidence — they aren't designed LUTs.

What actually happens: the pointer-table at `0x420022e0..0x420022f4` is read
by code around `0x420dee2f` (`l32r a8, 0x420022e0; callx8 a8`). The address
`0x3c236df6` stored there is being called as a function. ESP32-S3 maps the
same flash content through both the DROM window (`0x3c…`) and the IROM window
(`0x42…`), so these "LUT-shaped" tables at `0x3c236d24+` are actually code or
data structs being accessed via the DROM alias of an executable region. The
bytes happen to satisfy the UC8253 LUT format closely enough that the
controller accepts and acts on them, but with non-design drive intensities.

Conclusion: **the factory firmware does not contain a grayscale LUT set.**
The OEM never validated grayscale on this panel. Anyone pursuing grayscale
support needs to hand-tune LUTs from scratch — see the next section for the
recipe and constraints discovered on the bench.

## Hand-Tuned Grayscale LUT Recipe

Bench-tuning was done using a 4-stripe test probe at
`crosspoint-reader-main/src/murphy_grayscale_tuning_probe.cpp`. The probe
writes a known `(DTM1_bit, DTM2_bit)` pattern across four horizontal stripes
and lets the LUT bytes / power voltages be edited at the top of the file for
empirical calibration.

### Findings

**Voltage codes** (proven via the FAST B/W LUT path):

- `0x88` drives toward white (VSL polarity)
- `0x48` drives toward black (VSH polarity)
- `0x00` holds (no drive)
- Voltage codes appear in sub-phase byte positions 1-4 of each 7-byte phase
  block.

**Asymmetric rails.** VSH drives much harder per frame than VSL on this
panel. Any pattern with 2+ VSH (`0x48`) sub-phases collapses to solid black,
even paired with 2 VSL sub-phases. Pure VSL drive (`88 88 88 88`) only
reaches a light gray ("muddy white") at default voltages — not crisp white.

**LUT slot asymmetry.** The same byte pattern produces different intensities
depending on which LUT slot (`0x21`/`0x22`/`0x23`/`0x24`) it's loaded into.
Verified empirically: `88 88 88 88` lands at light gray in `LUT_22` but at
clean white in `LUT_23`. Byte tuning is per-slot — patterns can't be
transplanted between slots.

**DC bias drift.** Pure single-direction multi-phase LUTs (e.g. three phases
of `88 88 88 88`) drive pixels to the target initially, then cells slowly
drift back toward neutral gray over seconds as DC charge dissipates.
Compensating with VSH pulses doesn't work — saturated whites still respond
strongly to VSH and the compensation pulls them significantly back toward
black. The OEM kick+settle+drive structure is DC-balanced and produces stable
output, but on the asymmetric-rail Murphy panel it's only the structure used
in `LUT_24` that lands cleanly at white.

**Achievable intensity levels.** Three cleanly-distinguishable levels:

| Pattern (single-phase) | Result on Murphy |
| --- | --- |
| Any with 2+ `0x48` sub-phases | **Black** |
| `01 88 88 88 48 01 01` (3 white + 1 black) | **Dark gray** |
| `01 88 88 88 88 01 01` (pure white drive) | **Light gray** in `LUT_22`; **clean white** in `LUT_23` |
| OEM-default 3-phase `48/02/88` pattern in `LUT_24` | **Stable clean white** |

Light gray vs white differentiation is subtle on this panel. No usable
intermediate exists between dark gray and light gray.

### Final tuned voltages and LUTs

```
BOOSTER_SETTING = {0xD7, 0xDF, 0x1F}     // one notch above OEM VSL byte
VCOM_DC_SETTING = 0x03                   // alt-OEM VCOM (vs default 0x0F)
```

```cpp
// LUT_21 (slot fires for DTM1=1, DTM2=1) -> drive BLACK (DC-balanced)
constexpr uint8_t LUT_21[] = {
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01,  // negative kick
    0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01,  // settle
    0x01, 0x48, 0x48, 0x48, 0x48, 0x01, 0x01,  // positive drive (toward black)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_22 (slot fires for DTM1=0, DTM2=1) -> LIGHT GRAY (close to white)
constexpr uint8_t LUT_22[] = {
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_23 (slot fires for DTM1=1, DTM2=0) -> DARK GRAY
constexpr uint8_t LUT_23[] = {
    0x01, 0x88, 0x88, 0x88, 0x48, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// LUT_24 (slot fires for DTM1=0, DTM2=0) -> WHITE (DC-balanced)
constexpr uint8_t LUT_24[] = {
    0x01, 0x48, 0x48, 0x48, 0x48, 0x01, 0x01,  // positive kick
    0x01, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01,  // settle
    0x01, 0x88, 0x88, 0x88, 0x88, 0x01, 0x01,  // negative drive (toward white)
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

### Renderer encoding caveat

The renderer encodes 2-bpp values as `LSB→DTM1, MSB→DTM2`:

| Renderer value | (LSB, MSB) | Fires | Intensity with recipe above |
| ---: | --- | --- | --- |
| 0 (white) | (0,0) | LUT_24 | **white** ✓ |
| 1 (light mid) | (1,0) | LUT_23 | **dark gray** ⚠ inverted |
| 2 (dark mid) | (0,1) | LUT_22 | **light gray** ⚠ inverted |
| 3 (black) | (1,1) | LUT_21 | **black** ✓ |

The mid-tones are inverted relative to renderer expectation. To fix: swap
which plane goes to which DTM at the ingest layer (`LSB→DTM2, MSB→DTM1`).
That's a one-line change in `copyGrayscaleBuffers` / `copyGrayscale*Buffers`.

### Open levers (not yet swept)

- `VCOM_DATA_INTERVAL` (cmd `0x50`, currently `0x97`) — border behavior may
  affect post-drive idle drift.
- Booster bytes 0 (VSH ramp) and 2 (timing) — only byte 1 (VSL ramp) was
  swept.
- Multi-pass refresh: send the gray LUT twice in a row to accumulate drive.
- Try splitting drive duration via the `byte 0` slot of a phase block —
  earlier assumed it was a repeat-count, but `0x04` there broke drive. Could
  be TP source select rather than repeat.

## Practical Image Writer

Minimal full-image write path:

```cpp
resetPanel();
initUc8253OemSimple();
loadDefaultLuts();  // commands 0x20..0x24 with the MURPHY_LUT_*_DEFAULT tables

writeCommand(0x10);
writePlane(currentFrame, 12480);  // same buffer to both planes

writeCommand(0x13);
writePlane(currentFrame, 12480);

writeCommand(0x12);
waitBusyReadyHigh("display refresh", 12000);

writeCommand(0x02);
waitBusyReadyHigh("power off", 5000);
```

If polarity is inverted on the panel, invert every byte of `currentFrame` before sending. The visible "full inversion flash" before the image settles is intentional in these LUTs — phases drive positive then negative before holding at the destination value, which clears ghosting.

## Current Shipping State

**Murphy ships AA-disabled. Period.** The B/W FAST path with the periodic
ghost-clear refresh is the entire display story on this panel — full black
text on white background, no anti-aliasing pipeline involvement.

This decision is gated at the application layer via a new
`DeviceProfile::supportsGrayscaleAntiAlias` field (`false` for Murphy,
`true` for X3/X4) and a `ReaderUtils::effectiveAntiAlias()` helper that
combines the user's text-AA setting with the device capability. The
renderer's two-pass AA path (`copyGrayscaleLsbBuffers` +
`copyGrayscaleMsbBuffers` + `displayGrayBuffer`) never executes on Murphy,
whether the user enables the AA setting or not.

## Why AA Was Ripped Out

Every attempt to use the renderer's grayscale pipeline on this panel hit
one of these failure modes:

1. **Per-pixel pixelation.** The renderer produces detailed per-pixel
   `(LSB, MSB)` bit patterns at glyph edges. With our hand-tuned 4-LUT
   gray set, each `(DTM1, DTM2)` combination produces a *distinctly*
   different intensity. Adjacent pixels with different combinations look
   like a noisy quilt rather than a smooth AA gradient — the LUTs are
   doing their job correctly, but the panel's discrete intensity levels
   show up as visible per-pixel banding instead of smooth shading.

2. **Multi-refresh degenerate frames.** The renderer's two-pass AA flow
   calls `displayGrayBuffer` multiple times per page render, some passes
   with planes that are nearly all-zero or all-set. Our gray LUTs
   faithfully render each pass — including the degenerate ones — so the
   panel cycles through "correct text → fully inverted → all white" as
   each pass commits.

3. **`cleanupGrayscaleBuffers` fires SSD1677 commands.** The default SDK
   cleanup path writes to the SSD1677 RED-RAM (`writeRamBuffer(CMD_WRITE_RAM_RED, ...)`).
   UC8253 doesn't have that register; the commands corrupt controller state.

4. **Asymmetric VSH/VSL rails.** The panel's VSH (black-drive) rail is
   significantly stronger per frame than VSL (white-drive). DC-balanced
   waveforms net toward black; pure VSL drives reach the target but then
   drift back to gray as the cell's DC bias dissipates; VSH compensation
   pulses overshoot saturated whites toward black. The result is that
   reliably reaching crisp stable white is hard, and the dynamic range
   for intermediate grays is compressed.

The bench-tuning probe (`crosspoint-reader-main/src/murphy_grayscale_tuning_probe.cpp`)
*did* produce four clearly distinct horizontal stripes (black / dark gray /
light gray / white) with hand-tuned LUTs and bumped booster voltages,
proving the panel can technically do 4-level grayscale. But that proves
"uniform regions can hit four levels," not "text rendered with per-pixel
AA looks good." The two are very different problems on this hardware.

## Brief Soft-Text Experiment (Removed)

For a short window we shipped a "soft text" mode: kept the B/W FAST path
intact but swapped `LUT_24` (the "dark pixel" slot in same-buffer scheme)
from full black (`0x48 0x48 0x48 0x48`) to dark gray (`0x88 0x88 0x88 0x48`),
gated on `SETTINGS.textAntiAliasing`. This worked — uniform dark-gray text
on white, no AA pipeline, no per-pixel pixelation. But the visual benefit
was marginal (slightly softer text vs. crisp black) and not worth keeping
the extra SDK surface and tested-state. Removed.

## Future Work

If anyone picks this up again:

- The bench-tuning probe and findings here are the starting point. The
  4-LUT recipe with `BOOSTER = {0xD7, 0xDF, 0x1F}` and `VCOM_DC = 0x03`
  remains the best known stable state for producing 4 distinct uniform
  intensities.
- Real per-pixel AA would need a custom 1-pass grayscale refresh
  (bypassing the renderer's two-pass `copyGrayscale*` + `displayGrayBuffer`
  path entirely) plus a renderer-side change to emit per-pixel intensities
  in a single buffer rather than two planes.
- Asymmetric rails are a hardware property — no LUT trick gets us cleanly
  past that limit. The OEM didn't ship grayscale on this panel for the
  same reason.
- Worth investigating: the `0x17, 0xA5` partial-refresh trigger path
  documented in the OEM decompile, which could enable region-only updates
  without the ghost-clear cost. Orthogonal to grayscale but relevant for
  general refresh quality.
