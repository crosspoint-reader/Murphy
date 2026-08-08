# Murphy M4 — Hardware Findings

## Confidence Levels

- **CONFIRMED** — proven by direct measurement, esptool, or binary analysis
- **HIGH** — strong binary/string evidence, consistent with known hardware families
- **PENDING** — not yet probed; hardware probe sketches will confirm

---

## Processor & Memory

| Property | Value | Confidence |
|----------|-------|------------|
| SoC | ESP32-S3 (QFN56, revision v0.2) | CONFIRMED (esptool) |
| Cores | Dual Xtensa LX7, 240 MHz | CONFIRMED |
| Internal PSRAM | 8 MB (AP_3v3 package) | CONFIRMED (esptool) |
| External flash | 16 MB, quad SPI, 3.3V | CONFIRMED (esptool) |
| Crystal | 40 MHz | CONFIRMED |
| USB | Native USB-Serial/JTAG on GPIO19/20 | CONFIRMED |

The ESP32-S3 is Espressif's dual-core successor to the ESP32. Compared to the
ESP32-C3 used in the original Xteink X3/X4 readers (single-core RISC-V, 400 KB RAM),
the S3 is substantially more capable: more RAM, PSRAM support, faster cores, and
native USB.

---

## Display

| Property | Value | Confidence |
|----------|-------|------------|
| Resolution | 480 × 800 pixels | CONFIRMED (binary: gate lines = 479, SCK width) |
| Panel size | 4.26 inch | CONFIRMED (product + GoodDisplay panel match) |
| Panel model | GoodDisplay GDEQ0426T82 | CONFIRMED (binary LUT data matches exactly) |
| Controller | **SSD1677** | CONFIRMED (binary: init sequence + LUT data) |
| Interface | SPI (hardware SPI + GPIO CS/DC/RST/BUSY) | CONFIRMED (binary) |
| MOSI pin | GPIO3 | CONFIRMED (binary, constructor at VA 0x4208b034) |
| SCK pin | GPIO4 | CONFIRMED |
| CS pin | GPIO5 | CONFIRMED |
| DC pin | GPIO6 | CONFIRMED (LOW=command, HIGH=data) |
| RST pin | GPIO7 | CONFIRMED (active LOW, 2 ms pulse) |
| BUSY pin | GPIO8 | CONFIRMED (input, wait while HIGH) |
| Colour | Monochrome (black/white) | CONFIRMED |

These pins are **identical to the Murphy M3 display pins** despite most other
subsystems moving to different GPIOs in the M4.

Binary evidence — init command sequence at IROM VA `0x42189b48`:
- `0x12` = software reset (CMD_SOFT_RESET, SSD1677-specific)
- `0x0C` + 5-byte booster data `AE C7 C3 C0 80` (SSD1677 booster soft-start)
- `0x01` + gate count = 479 (480 rows − 1)
- `0x3C` = border waveform control
- LUT_FAST data matches GxEPD2_426_GDEQ0426T82 byte-for-byte

### What is an e-ink controller?

E-ink panels are not like LCD screens. They have no internal memory or refresh
logic — the host microcontroller must push every pixel and manage complex
voltage waveforms. The controller chip (UC8253, SSD1677, etc.) acts as the
intermediary: it accepts pixel data over SPI, manages the high-voltage charge
pumps, and applies the waveform sequences that physically move the charged
particles inside the display to create an image.

The SSD1677 differs from the M3's UC8253 in register layout and LUT format.
A new display driver targeting the SSD1677 and the GDEQ0426T82 panel's
specific waveform tables will be needed in the freeink-sdk.

---

## Touch Controller

| Property | Value | Confidence |
|----------|-------|------------|
| IC | Factory-labelled FT6336U-compatible controller | HIGH (factory binary label; package not inspected) |
| I2C address | `0x2E` | CONFIRMED (working implementation and factory binary) |
| SDA pin | GPIO13 | CONFIRMED (working implementation and factory binary) |
| SCL pin | GPIO12 | CONFIRMED (working implementation and factory binary) |
| INT pin | GPIO44 | CONFIRMED (power-detect probe) |
| Power enable | GPIO45 | CONFIRMED (power-detect probe, active LOW) |

The touch IC is a capacitive controller. It communicates over I2C and
reports touch coordinates and gesture events. The INT pin (GPIO44) is driven
LOW by the FT6336U when a touch event is ready — the host reads coordinates
from I2C then waits for INT to go HIGH again before the next read.

GPIO45 controls the touch controller's power via a PMOS transistor (active LOW —
driving GPIO45 LOW turns on its supply rail). This was discovered using the
power-detect probe: driving GPIO45 LOW caused GPIO44 to rise from LOW to HIGH
(the INT pull-up rail powered up), which revealed the power/INT relationship.

Unlike the M3's CHSC6x, this controller required a new FreeInk SDK driver. See
[touch.md](touch.md) for the working initialization, protocol, and safety boundary.

An early generic scan appeared to find `0x38` on GPIO10/GPIO4, but that path
never produced valid touch data. The factory-derived GPIO13/GPIO12, address
`0x2E` path is the confirmed implementation.

Binary evidence from `murphy-26-0526-1.2.16.bin`:
```
Mofei FT6336U config failed on SDA=%d SCL=%d addr=0x%02X
```

---

## SD Card

| Property | Value | Confidence |
|----------|-------|------------|
| Interface | 4-bit SD_MMC (SDIO) | CONFIRMED |
| CLK pin | GPIO16 | CONFIRMED (sd_mount_confirm probe) |
| CMD pin | GPIO15 | CONFIRMED |
| D0 pin | GPIO17 | CONFIRMED |
| D1 pin | GPIO18 | CONFIRMED |
| D2 pin | GPIO11 | CONFIRMED |
| D3 pin | GPIO14 | CONFIRMED |
| Power enable | GPIO10 | CONFIRMED (active LOW) |
| Card capacity (test card) | 29.8 GB | CONFIRMED (mounted) |
| Working frequency | 400 kHz (4-bit) | CONFIRMED |

Binary evidence from `murphy-26-0526-1.2.16.bin` that pointed at this:
```
Mofei SD_MMC (4-bit) setPins failed
```

Factory firmware `setPins` call disassembled at VA `0x42008052` (Xtensa LX7):
```asm
movi.n a11, 16   ; clk
movi.n a12, 15   ; cmd
movi.n a13, 17   ; d0
movi.n a14, 18   ; d1
movi.n a15, 11   ; d2
movi.n a8,  14   ; d3 (pushed to stack as 7th arg)
call8  0x42069b98  ; SDMMCFS::setPins
```

### How the SD power circuit works

The SD data lines (CMD, D0–D3) have pull-up resistors that connect to SD_VCC —
a switched power rail, not the main 3.3V supply. When SD_VCC is off, these
lines float LOW despite having pull-ups. When SD_VCC turns on, all five lines
immediately rise to HIGH.

GPIO10 drives a PMOS transistor gate (active LOW). When GPIO10 is driven LOW
as an output, the PMOS turns on and connects SD_VCC to the main rail. When
GPIO10 is HIGH (or left as input with pull-up), the PMOS is off and the card
has no power.

GPIO10 is dedicated to the SD power gate in the confirmed port. Earlier notes
that treated it as touch SDA were based on a non-working probe result.

### Why this took many probe iterations

The SD investigation lasted many rounds because of several compounding issues:

**1. GPIO10 was excluded from power-detect testing.** Our `sd_power_detect`
probe hardcoded a candidate list of HIGH-baseline pins to test as power enable.
GPIO10 was HIGH (I2C SDA pull-up holds it HIGH), but we excluded it because
we already "knew" it was I2C SDA. This was the critical miss — the very pin
we ruled out was the answer.

**2. Back-driving through pull-up resistors.** When an SD data pin is driven
HIGH as an ESP32 output, current flows through its ~10 kΩ pull-up into the
SD_VCC node. This charges SD_VCC enough that the other data pins (sharing
the same pull-up network) also appear HIGH. Any SD pin driven HIGH thus
"looked like" the power enable — both GPIO15 and GPIO16 triggered this
artifact when driven HIGH, producing 5 rising pins and falsely appearing to
be the power gate.

**3. GPIO15 reserved as power pin in probes v5–v6.** Because of the
back-driving artifact, GPIO15 was incorrectly identified as the power enable
and removed from the CMD/D0 search space. But CMD=GPIO15 — so the correct
combination (CLK=16, CMD=15, D0=17) was never tried.

**4. Final resolution via firmware binary analysis.** Disassembling the
factory firmware's Xtensa LX7 code revealed the exact `setPins` arguments
and the `gpio_set_level(10, 0)` power-on call, which identified GPIO10 as
the power pin and confirmed all six SD interface pins.

4-bit SD_MMC is faster than the SPI-mode SD used on M3. The ESP32-S3 has a
dedicated SDMMC peripheral that handles the SDIO protocol in hardware.
The freeink-sdk already has SDMMC support (used by the de-link board).

---

## Frontlight

| Property | Value | Confidence |
|----------|-------|------------|
| Channels | 2 (cool white + warm white) | CONFIRMED (hardware test) |
| Cool GPIO | GPIO47 | CONFIRMED (frontlight probe) |
| Warm GPIO | GPIO48 | CONFIRMED (frontlight probe) |
| PWM | 25 kHz, 10-bit, active high | CONFIRMED (hardware test) |

Binary evidence from `murphy-26-0526-1.2.16.bin`:
```
frontlightCool
frontlightWarm
```

The dual-tone frontlight lets users adjust colour temperature between a
blue-white (cool) and an amber-yellow (warm) tone, which is important for
comfortable reading in low light. Each channel is a separate LED array driven
by its own GPIO with PWM dimming.

---

## Buttons

| Button | GPIO | Confidence |
|--------|------|------------|
| Top side button | GPIO1 | CONFIRMED (button_probe — goes LOW when pressed) |
| Middle side button | GPIO2 | CONFIRMED (button_probe — goes LOW when pressed) |
| Bottom side button | GPIO0 | CONFIRMED (boot strapping pin + power/function button) |
| Recessed RESET | RST (CHIP_PU) | Hardware reset — not a GPIO, no mapping needed |

All three buttons are active LOW — pressing connects the line to GND, releasing
lets a pull-up resistor return it to HIGH.

GPIO0 is the ESP32-S3 strapping pin: holding it LOW during reset forces download
mode, which is how we flash probes. During normal firmware operation it behaves
as a regular active-LOW input. It also serves as the power button on M4 (the
factory firmware uses a long-press to sleep/wake), so CrossPoint will need to
handle both short-press (function) and long-press (power) on GPIO0.

The recessed RESET button connects directly to the CHIP_PU (chip enable) pin —
pulling it LOW immediately resets the CPU. There is no GPIO involved and no
firmware mapping required.

---

## Audio

Audio hardware has not been confirmed and remains out of scope for the initial port.

---

## WiFi / Bluetooth

Built into the ESP32-S3. The factory firmware uses WiFi for OTA updates
(connects to `murphy.pandacat.ai`). CrossPoint will retain WiFi for its
own network features (OPDS, Calibre sync, etc.).

---

## OTA Update Mechanism

The factory firmware fetches updates from `http://murphy.pandacat.ai/ota/latest`.
The endpoint returns a JSON manifest:
```json
{ "firmware_url": "...", "firmware_size": ... }
```
The device then downloads the binary and writes it to the `app1` OTA slot.
This mechanism can be intercepted by running a fake OTA server on a local
network — useful as an alternative to USB flashing.
