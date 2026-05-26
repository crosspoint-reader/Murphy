# Murphy M3 Display GPIO Recovery

Created: 2026-05-26

Scope: Murphy M3 only. This is an ESP32-S3 QFN56 rev v0.2 device with 40 MHz crystal, dual core 240 MHz, Wi-Fi/BT LE, and 8 MB embedded PSRAM. Do not mix these findings with Xteink X3/X4 stock firmware notes.

## Executive Result

Best current OEM-derived screen mapping:

| Signal | GPIO | Confidence | Notes |
| --- | ---: | --- | --- |
| EPD MOSI / SDA | `3` | High | OEM byte writer shifts each bit by writing GPIO3. |
| EPD SCK / CLK | `4` | High | OEM byte writer toggles GPIO4 low/high around each bit. |
| EPD CS | `5` | High | OEM command/data wrappers pull GPIO5 low around transfers. |
| EPD DC | `6` | High | OEM command wrapper drives GPIO6 low for command phase, then high for data/idle. |
| EPD RST | `7` | High | OEM reset helper pulses the constructor field stored at object offset `0x14`. Constructor passes GPIO7 there. |
| EPD BUSY | `8` | High | OEM init waits until the constructor field stored at object offset `0x16` reads high. Constructor passes GPIO8 there. |

Use this first probe tuple:

```text
SCK=4 MOSI=3 CS=5 DC=6 RST=7 BUSY=8
BUSY polarity: ready-high
Transport: bit-banged first, matching OEM behavior
```

Known non-display pins from current device work:

| GPIO | Finding |
| ---: | --- |
| `0`, `1`, `2` | Button-side observations; do not use for display first. |
| `48` | Confirmed front-light PWM, active high. Not display BUSY. |

## Why This Matters

The public CrowPanel-style mappings are wrong for this Murphy unit. Earlier public maps used `SCK12/MOSI11` and often `BUSY48`, but OEM decomp points to a bit-banged display bus on GPIO3/GPIO4 and confirms GPIO48 is not BUSY.

The display failure should now be treated as a Murphy board-profile mismatch below the CrossPoint UI layer, not as an X4 renderer/display-driver issue.

## Evidence Summary

### Constructor Call

The stock firmware calls the display constructor at `0x42022618`.

Just before the call:

- `a11 = 5`
- `a12 = 6`
- `a13 = 7`
- `a14 = 8`

Under Xtensa windowed-call argument mapping, the callee receives these as constructor GPIO arguments `5, 6, 7, 8`.

Evidence:

- `analysis/display_constructor_call_disasm_20260526.md:21-34`
- `analysis/display_constructor_call_disasm_20260526.md:72-79`

### Constructor Object Layout

The constructor at `0x42038280` stores those arguments into display object fields:

| Object offset | Constructor arg | Later use |
| ---: | ---: | --- |
| `0x10` | `5` | CS |
| `0x12` | `6` | DC |
| `0x14` | `7` | RST |
| `0x16` | `8` | BUSY |

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:7-31`

### Bit-Banged Data Lines

The OEM byte writer at `0x42038648` does:

```text
GPIO4 low
GPIO3 = next data bit
GPIO4 high
repeat 8 bits
GPIO4 low
```

This maps:

- `GPIO3` = MOSI/SDA
- `GPIO4` = SCK/CLK

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:203-220`

### CS And DC

The command wrapper at `0x4203868c` does:

```text
GPIO6 low
GPIO5 low
write byte
GPIO5 high
GPIO6 high
```

This maps:

- `GPIO5` = CS
- `GPIO6` = DC

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:227-239`

The higher-level command/data helpers confirm the same object fields:

- Command write pulls object offset `0x12` low, object offset `0x10` low, writes byte, then raises `0x10` and `0x12`.
- Data write pulls object offset `0x10` low, writes byte, then raises `0x10`.
- Multi-data write uses command phase followed by data bytes with the same `0x10`/`0x12` behavior.

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:75-99`
- `analysis/display_driver_core_decompile_20260526.md:104-121`
- `analysis/display_driver_core_decompile_20260526.md:127-145`
- `analysis/display_driver_core_decompile_20260526.md:151-174`

### Reset

The reset helper at `0x420382fc` pulses the object field at offset `0x14`. The constructor placed GPIO7 in that field.

This maps:

- `GPIO7` = RST

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:40-69`
- `analysis/display_driver_core_decompile_20260526.md:27-31`

Important correction: the public CrowPanel docs list GPIO7 as a display power enable, but the Murphy OEM display driver uses GPIO7 as reset.

### BUSY

The init helper at `0x420389ec` waits while the object field at offset `0x16` reads zero. The constructor placed GPIO8 in that field.

This maps:

- `GPIO8` = BUSY
- BUSY ready polarity appears to be high

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:335-338`
- `analysis/display_driver_core_decompile_20260526.md:27-31`

### GPIO Mode Setup

The OEM display setup function configures GPIO8, then GPIO7, GPIO6, GPIO5, GPIO4, and GPIO3.

This independently matches the recovered bus:

```text
GPIO8 BUSY
GPIO7 RST
GPIO6 DC
GPIO5 CS
GPIO4 SCK
GPIO3 MOSI/SDA
```

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:275-288`

## OEM Init And Update Behavior

The OEM path is UC8253-like and uses commands that match the known panel family:

| Function | Meaning | Key commands |
| --- | --- | --- |
| `FUN_420389ec` | Display init | `0x01`, `0x06`, power-on, `0x00`, `0x30`, `0x61`, `0x82`, `0x50` |
| `FUN_420389cc` | Power on | command `0x04`, waits/logs `_PowerOn` |
| `FUN_42038f24` | Power off | command `0x02`, waits/logs `_PowerOff` |
| `FUN_42038b60` | LUT/data table load | commands `0x20..0x24` |
| `FUN_42038f74` | Fast update | command `0x12`, waits/logs `_Update_Fast` |
| `FUN_42038fa4` | Partial update | command `0x12` or `0x17/0xa5`, waits/logs `_Update_Part` |

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:315-370`
- `analysis/display_driver_core_decompile_20260526.md:396-430`
- `analysis/display_power_task_decompile_20260526.md:3-30`

The constructor uses dimensions `0xf0` and `0x1a0`, matching `240 x 416` orientation for a `416 x 240` class panel.

Evidence:

- `analysis/display_driver_core_decompile_20260526.md:295-308`
- `analysis/display_constructor_call_disasm_20260526.md:59-64`

## Related Strings And Non-GPIO Findings

The OEM image contains display-related strings and pointer references:

- `_PowerOn`
- `_PowerOff`
- `_Update_Fast`
- `_Update_Part`
- `displayOtaTask`
- `Not full refresh`
- `LUT_DEL`
- `LUT_ADD`
- `LUT_FULL_ADD`
- `/lut.bin`
- `/lut_full.bin`

Findings:

- `_PowerOn` points to `FUN_420389cc`.
- `_PowerOff` points to `FUN_42038f24`.
- `displayOtaTask` points to `FUN_4201482c`, which creates a task and appears to handle display OTA/upload progress, not the panel GPIO bus itself.
- LUT strings sit in pointer/literal tables, but did not directly reveal a separate GPIO map.

Evidence:

- `analysis/display_power_epd_pointer_refs_20260526.md:6-40`
- `analysis/display_power_epd_pointer_refs_20260526.md:42-130`
- `analysis/display_power_task_decompile_20260526.md:81-133`

## Rejected Or Superseded Public Mappings

Do not prioritize these until the OEM tuple above has been tested:

| Source | Mapping | Status |
| --- | --- | --- |
| Public CrowPanel primary | `SCK12 MOSI11 CS45 DC46 RST47 BUSY48` | Superseded. GPIO48 is front light on Murphy. |
| Public CrowPanel alternate | `SCK12 MOSI11 CS10 DC9 RST21 BUSY48` | Superseded. GPIO48 is front light on Murphy. |
| Good Display sample adapter | `SCK12 MOSI11 CS18 DC17 RST16 BUSY15` or raw `CS17 DC16 RST15 BUSY14` | Adapter-board assumption, not Murphy OEM mapping. |
| Waveshare comparator | `SCK11 MOSI12 CS10 DC9 RST46 BUSY3` | Comparator only, not supported by OEM decomp. |

Evidence:

- `findings/codex_handoff.md:37-47`
- `findings/display_bringup.md:26-39`

## Immediate Probe Plan

1. Add one Murphy-only standalone display probe case:

```text
SCK=4 MOSI=3 CS=5 DC=6 RST=7 BUSY=8
BUSY ready-high
```

2. Bit-bang the bus first, matching OEM behavior. Do not start with ESP-IDF SPI bus initialization.

3. Use the OEM init sequence as the first command reference:

```text
0x01: 03 10 3F 3B 0D
0x06: D7 D7 1F
0x04: power on and wait ready
0x00: FF
0x30: 09
0x61: F0 01 A0
0x82: 0F
0x50: 97
```

4. Use visible screen change as the only success signal. Do not accept audible clicks or BUSY readings alone as success.

5. If this tuple still does not move the panel, continue decomp around the constructor caller at `0x420225ec..0x4202262c` to look for an additional rail/enable sequence. Do not random-sweep GPIOs.

## Files Generated In This Pass

Primary consolidated finding:

- `findings/display_gpio_recovery.md`

Raw artifacts:

- `analysis/display_driver_core_decompile_20260526.md`
- `analysis/display_constructor_refs_20260526.md`
- `analysis/display_constructor_call_disasm_20260526.md`
- `analysis/display_power_epd_pointer_refs_20260526.md`
- `analysis/display_power_task_decompile_20260526.md`
- `analysis/display_exact_string_refs_20260526.md`
- `analysis/display_focused_range_refs_20260526.md`
- `analysis/display_literal_users_20260526.md`
- `analysis/display_table_range_refs_20260526.md`

Tooling added:

- `tools/ghidra_scripts/FindAddressRangeRefs.java`

## Bottom Line

For the screen, use:

```text
MOSI=3
SCK=4
CS=5
DC=6
RST=7
BUSY=8
```

Keep GPIO0/1/2 for buttons and GPIO48 for front light.
