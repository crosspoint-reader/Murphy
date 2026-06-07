# Murphy M3 Display Hardware

Scope: Murphy M3 only (ESP32-S3 QFN56 rev v0.2, 40 MHz crystal, dual core 240 MHz, 8 MB embedded PSRAM). Do not mix with Xteink X3/X4 stock firmware notes.

Function-level Ghidra evidence and the OEM init/LUT/refresh sequences live in [Display Driver and Refresh Path](display_driver.md). This document covers panel identity and the GPIO bus.

## Panel

- Good Display `GDEY037T03-FT21`
- 3.7-inch e-paper with touch and front light
- UC8253 controller
- 416 x 240 resolution, addressed as 240 x 416 in OEM constructor (`0xf0` x `0x1a0`)
- SPI display interface, bit-banged by OEM firmware
- 24-pin display FPC, 6-pin FT-style touch FPC, 6-pin front-light FPC

Physical markings observed on this unit:

- `YSFK082048A-W-3`
- `LH37004D 2513`
- small FPC label appears to begin with `FPC F...`

No useful public indexed match was found for `YSFK082048A-W-3` or `LH37004D`. Treat these as module/FPC production markings for the `GDEY037T03-FT21` assembly, not necessarily as public product numbers.

## Confirmed GPIO Bus

OEM-derived bit-banged bus, confirmed on hardware (CrossPoint boot/home UI renders on this map):

| Signal | GPIO | Evidence |
| --- | ---: | --- |
| EPD MOSI / SDA | `3` | OEM byte writer `FUN_42038648` shifts each bit by writing GPIO3. |
| EPD SCK / CLK | `4` | OEM byte writer toggles GPIO4 low/high around each bit. |
| EPD CS | `5` | Command wrapper at `FUN_4203868c` pulls GPIO5 low around transfers. |
| EPD DC | `6` | Command wrapper drives GPIO6 low for command phase, high for data/idle. |
| EPD RST | `7` | Constructor stores reset pin in object offset `0x14`; reset helper `FUN_420382fc` pulses it. |
| EPD BUSY | `8` | Constructor stores busy pin in object offset `0x16`; init waits while `digitalRead(BUSY) == 0` (ready-high polarity). |

First probe tuple, ready-to-use:

```text
SCK=4 MOSI=3 CS=5 DC=6 RST=7 BUSY=8
BUSY polarity: ready-high
Transport: bit-banged (match OEM)
```

Independent confirmation: the OEM GPIO mode-setup function configures GPIO8, then GPIO7/6/5/4/3 in order, matching the recovered bus.

See [Display Driver and Refresh Path](display_driver.md) for the constructor chain, byte-writer source, and full evidence with analysis-file line references.

## Reserved or Superseded Pins

| GPIO | Status |
| ---: | --- |
| `0`, `1`, `2` | Confirmed physical buttons. Do not use for display. |
| `48` | Confirmed front-light PWM, active high. Not display BUSY. |

Superseded public-reference mappings — kept here for historical context and to flag any external docs that still cite them:

| Source | Mapping | Why rejected |
| --- | --- | --- |
| Public CrowPanel primary | `SCK12 MOSI11 CS45 DC46 RST47 BUSY48` | GPIO48 is front light on Murphy. |
| Public CrowPanel alternate | `SCK12 MOSI11 CS10 DC9 RST21 BUSY48` | Same. |
| Good Display sample adapter | `SCK12 MOSI11 CS18 DC17 RST16 BUSY15` (raw: `CS17 DC16 RST15 BUSY14`) | Adapter-board assumption, not Murphy wiring. Probe runs showed BUSY15 stuck high through reset/power/refresh. |
| Waveshare comparator | `SCK11 MOSI12 CS10 DC9 RST46 BUSY3` | Comparator only; OEM decomp does not support this map. BUSY3 stuck low in probe runs. |

## Why Static RE Was Slow

The OEM driver has almost no helpful display strings near pin setup. Early searches biased toward strings, public board maps, and obvious GPIO constants found UI features and false leads but missed the bus. The path that worked:

1. Find the UC8253 command cluster (`0x01`, `0x06`, `0x61`, `0x82`, `0x50`, `0x10`, `0x13`, `0x12`).
2. Follow calls down to the command/data byte writers.
3. Observe direct `digitalWrite()` calls on GPIO3/4/5/6.
4. Follow constructor object fields back to CS/DC/RST/BUSY.
5. Confirm with a standalone GPIO3-8 probe on hardware.

Probe source: `m3/probes/murphy_epd_gpio3_8_probe.cpp` at the repo root.
