# OEM grayscale/refresh LUTs extracted from `touch v525`

## Summary

The corogoo MoFei firmware build that the device's OTA URL actually
points to (`firmware/touch/firmware.bin`, version 525, dated 2026-01-14)
contains a **504-byte block of compiled-in UC8253 LUT data** that is
byte-for-byte identical to the corresponding block in the device's own
app0 dump.

This is the OEM's stock LUT recipe — the one that produces the
bench-observed behavior on Murphy hardware — and was the missing piece
in the earlier reverse-engineering documented in
`findings/display_driver.md`.

## Where it lives

| | File offset | Virtual address |
|---|---|---|
| `mofei-corogoo-touch-v525.bin` | `0x000ab7e0` – `0x000ab9d8` | `0x3c23b7e0` (DROM) |
| `analysis/extracted/app0.bin` (device) | `0x000a6ea0` – `0x000a7098` | (similar DROM region) |

Confirmed: a `find()` of the 504-byte slice from touch v525 inside the
device's app0.bin returns a direct hit. The two builds share the same
LUT source.

Neither `mofei-corogoo-EPD426-v1.bin` (gitee `EPD426-v1/firmware.bin`,
version 621) nor `murphy-26-0526-1.2.16.bin` (pandacat Murphy Reader)
contains this byte pattern — those firmwares load LUTs from filesystem
files instead (see "Runtime LUT files" below).

## What's in the block

Ten 42-byte UC8253 LUT register payloads, each pointed at by exactly one
`l32r` literal in the code segment (verified via 4-byte LE literal search
in the code segments). Saved per-LUT as raw `.bin` files plus a C header
under `analysis/upstream_murphy_reader/oem_luts/oem_touch_v525_luts.h`.

The pointer targets the **start** of each 42-byte payload (active phase
data first, zero padding at end):

| File offset | Phase-1 → Phase-3 voltage | Likely UC8253 slot |
|---|---|---|
| `0xab824` | `0x88 → 0x48` (drive white, end black) | LUT_23 (WB) or LUT_24 (BB) |
| `0xab84e` | `0x88 → 0x48` | duplicate of above |
| `0xab878` | `0x48 → 0x88` (drive black, end white) | LUT_21 (WW) or LUT_22 (BW) |
| `0xab8a2` | `0x48 → 0x88` | duplicate of above |
| `0xab8cc` | `0x08 → 0x08` (VCOM tracking) | LUT_20 (LUTC / VCOM) |
| `0xab8f6` | single phase `0x0f 0x8f 0x4f` | voltage config |
| `0xab920` | single phase `0x4f 0x8f 0x4f` | voltage config |
| `0xab94a` | single phase `0x0f 0x8f 0x0f` | voltage config |
| `0xab982` | single phase `0x4f 0x8f 0x0f` | voltage config |
| `0xab9ac` | single phase `0x0f 0x0f 0x0f` | voltage config (all-discharge?) |

### LUT phase decode

Within each refresh LUT (`lut21..lut24`, `lutc`), the meaningful 21-byte
payload at the end is:

```
01  V1 V1 V1 V1  01 01  01 02 02 02 02  01 01  01  V2 V2 V2 V2  01 01
^   ^^^^^^^^^^^  ^^^^^  ^^^^^^^^^^^^^  ^^^^^  ^   ^^^^^^^^^^^  ^^^^^
|   phase A      gate   transition      gate   |   phase B      gate
|   voltage      ctrl   frame counts     ctrl  |   voltage      ctrl
start                                          mid
```

For example, `lut21_ww_white_drive`:

```
01 88 88 88 88 01 01  →  Phase A: 4 frames at VSL (white drive)
01 02 02 02 02 01 01  →  Settle: 8 transition frames
01 48 48 48 48 01 01  →  Phase B: 4 frames at VSH (black drive)
```

This is push-pull settle — exactly the pattern we bench-tuned in
`display_driver.md`, except the OEM uses a 4-4-4 frame split where
we landed on slightly different counts. Using these OEM frame counts as
the baseline before any further tuning is now the right starting point.

`lutc_vcom` uses the `0x08` code (VCOM tracking, no drive) in both phases:
the VCOM channel doesn't swing, only the cell electrodes do — confirming
the symmetric drive scheme.

The `0x88`/`0x48` voltage codes are the same ones we identified by
probe in `findings/display_driver.md`:
`0x88` = VSL/white drive, `0x48` = VSH/black drive.

### Power/voltage config blocks

The five `volt_*` blocks contain only a single 3-byte voltage tuple
followed by the standard `01 01 01` gate-control bytes. They are
probably the arguments to UC8253 register `0x01` (POWER_SETTING) or
`0x82` (VCOM_DC), one per refresh mode (fast/full/init/etc.). The
distinct values (`0f 8f 4f`, `4f 8f 4f`, `0f 8f 0f`, `4f 8f 0f`,
`0f 0f 0f`) suggest the OEM switches voltage rail configuration
depending on the refresh path it's running.

`0x0f` = `0000 1111` (all four sub-slots driven), `0x4f` = `0100 1111`
(adds VSH bit), `0x8f` = `1000 1111` (adds VSL bit). The `0f0f0f`
variant is the "all-discharge" / sleep-discharge config.

## Runtime LUT files (`/lut.bin`, `/lut_full.bin`)

The touch v525 binary also references two LittleFS paths:

```
/lut.bin
/lut_full.bin
```

and exposes USB protocol commands:

```
LUT_ADD
LUT_ADD OK
LUT_DEL
LUT_FULL_ADD
LUT_FULL_ADD OK
```

These let the OEM (or any user with the right host tool) upload
alternate LUTs to the device's flash filesystem at runtime, separately
for partial and full refresh. The compiled-in 504-byte block above is
the **default / fallback / partial-refresh** LUT; whatever is in
`/lut_full.bin` on the device's LittleFS would override it for full
refresh.

Our `analysis/extracted/spiffs.bin` is entirely `0xff` (erased before
dump), so we don't have the on-device files. If a fresh dump is taken
on a working unit, `/.mofei/lut.bin` and `/.mofei/lut_full.bin` (path
varies between MoFei branches) will hold the actually-active LUTs.

## Why EPD426-v1 and Murphy v1.2.16 don't have this block

The newer EPD426-v1 (gitee, version 621) and Murphy Reader v1.2.16
(pandacat) firmwares don't contain the 504-byte signature. Combined
with the runtime `LUT_ADD` infrastructure, the most likely explanation
is that those builds dropped the compiled-in default and now always
load LUTs from the filesystem. EPD426-v1 also ships the "Gray Refresh
Test" diagnostic (see `upstream_mofei_firmware.md`) — the diagnostic
exists *because* the LUT is now data-driven and needs to be validated
after upload.

## Implications for the SDK port

After re-aligning the extraction, the OEM 42-byte LUT register
payloads are **byte-for-byte identical to the existing
`MURPHY_LUT_*_DEFAULT` constants** in
`community-sdk/libs/display/EInkDisplay/src/EInkDisplay.cpp`. Direct
verification:

| SDK constant | First OEM offset that matches |
|---|---|
| `MURPHY_LUT_20_DEFAULT` (VCOM) | `0x000ab8cc` |
| `MURPHY_LUT_21_DEFAULT` | `0x000ab878` (also `0x000ab8a2`) |
| `MURPHY_LUT_22_DEFAULT` | `0x000ab878` (also `0x000ab8a2`) |
| `MURPHY_LUT_23_DEFAULT` | `0x000ab824` (also `0x000ab84e`) |
| `MURPHY_LUT_24_DEFAULT` | `0x000ab824` (also `0x000ab84e`) |

So there is **no LUT-byte change to make** in the SDK — the bytes we
shipped are the OEM's. The cited source has been recorded as a comment
above `MURPHY_LUT_20_DEFAULT` in `EInkDisplay.cpp`.

What this leaves still open:

1. **Grayscale AA on Murphy.** The OEM compiled-in LUTs are the same
   ones we already use, and they don't contain a grayscale-specific
   LUT — only B/W refresh. Re-enabling AA in `DeviceProfile` based on
   this extraction alone is unjustified, because nothing about the
   underlying LUT data has changed.
2. **The actually-active grayscale LUT** in the OEM is presumably
   loaded from `/lut_full.bin` (or `/lut.bin`) on LittleFS at runtime,
   via the `LUT_ADD`/`LUT_FULL_ADD` USB protocol. Our extracted
   `spiffs.bin` is all `0xff` (erased before dump). To get the real
   grayscale LUT, the right move is to take a fresh dump of `/.mofei/`
   from a powered-on, working Murphy unit — or capture the SPI traffic
   during a known-grayscale refresh.
3. **The five voltage config blocks** (`0f8f4f`, `4f8f4f`, `0f8f0f`,
   `4f8f0f`, `0f0f0f`) are new info compared to the SDK's current
   single `MURPHY_OEM_POWER_SETTING_ALT` constant. They're probably
   fed into a UC8253 power/VCOM register at different refresh modes.
   Worth tracing the call sites in Ghidra to confirm which command
   byte they're written under before pulling them into the SDK.

## Saved artifacts

```
analysis/upstream_murphy_reader/
  mofei-corogoo-touch-v525.bin
  oem_touch_v525_lut_block.bin              (504 bytes — full DROM region)
  oem_luts/
    lut_20_vcom.bin                         (42 bytes — VCOM tracking)
    lut_21_ww_or_22_bw_first.bin            (42 bytes — dst 0x88, slot A)
    lut_21_ww_or_22_bw_second.bin           (42 bytes — dst 0x88, slot B)
    lut_23_wb_or_24_bb_first.bin            (42 bytes — dst 0x48, slot A)
    lut_23_wb_or_24_bb_second.bin           (42 bytes — dst 0x48, slot B)
    volt_0f8f4f.bin   volt_4f8f4f.bin
    volt_0f8f0f.bin   volt_4f8f0f.bin
    volt_0f0f0f.bin
    oem_touch_v525_luts.h                   (C header bundling all above)
```
