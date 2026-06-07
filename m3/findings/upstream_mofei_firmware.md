# Upstream Project: MoFei / corogoo

The device's app image and firmware track an open-source upstream
published on Gitee under:

  https://gitee.com/corogoo/3.7-inch-ink-screen-reader

The repo itself has no source code — the README is the empty Gitee
template — but the `firmware/` directory hosts compiled binaries for
two distinct hardware tracks. All blobs share the same compile
timestamp (`Mar  5 2024 12:12:53`), the same `arduino-lib-builder`
project name, and the same ESP-IDF `v4.4.7-dirty` toolchain, but they
have distinct ELF SHA-256 values and different feature sets.

## Identity strings

- App name in OTA UI: **MoFei** (墨非 / Pinyin for "Murphy")
- mDNS host: `mofei.local`
- USB MSC volume label: `MOFEI Storage`
- On-device data directory: `/.mofei/` (epub cache, `read_history.json`,
  `lock_screen_pic.bin`)

## Tracks

| Track | Gitee path | Version | Date | Local copy |
|---|---|---|---|---|
| `touch` | `firmware/touch/firmware.bin` | 525 | 2026-01-14 | `m3/oem_firmware/mofei-corogoo-touch-v525.bin` |
| `EPD426-v1` | `firmware/EPD426-v1/firmware.bin` | 621 | 2026-05-19 | `m3/oem_firmware/mofei-corogoo-EPD426-v1.bin` |

The `update.json` URL embedded in the device's dumped app0 image
points at the **`touch/` branch** — that is the track this hardware is
on. `EPD426-v1` is a newer, larger sibling that runs on a related (but
not identical) form factor; not what ships to Murphy M3 owners over OTA.

## Relationship to the device dump

`m3/extracted/app0.bin` (the app pulled off Murphy M3 hardware)
and `mofei-corogoo-touch-v525.bin` share:

- Identical compile timestamp / ESP-IDF / project name
- The same 504-byte UC8253 LUT block (see
  `oem_touch_v525_grayscale_luts.md`)
- The same EPUB/reader log-format strings (`[EPUB][reader] cache
  no-progress ...`, `[EPUB][chapter] parse start tocIndex=...`,
  `[READER] page render %s: %lu ms`, etc.)

They have **different ELF SHA-256s**:
- Device app0: `a6f205d9e091d605cf2d0e1684ee43f08ae63ff52edcdd3becbf81d62563ca3b`
- Touch v525:  `54a62a86e269e08cc8eb0196d9e7f20eb2d33a6208cb2f26f21b4f1a999e96a7`

So the device shipped with an earlier sibling build than what the OTA
URL now serves. Touch v525 is the closest publicly-available reference
firmware for the device.

## What this means for the SDK port

- The OEM's compiled-in LUTs from `touch/firmware.bin` match the LUTs
  on the device byte-for-byte, and they match the SDK's existing
  `MURPHY_LUT_*_DEFAULT` constants — see
  `oem_touch_v525_grayscale_luts.md` for the byte-level comparison.
- The `EPD426-v1` build adds a "Gray Refresh Test" diagnostic
  (`Tap screen to draw black circle` / `Expected: black | dark gray |
  light gray | white`) and references runtime-loaded `/lut.bin` /
  `/lut_full.bin` files. The `touch` build the device runs does
  **not** ship a grayscale diag and has no compiled-in grayscale LUT.
  Whether the panel can do clean 4-level grayscale at all on this
  hardware track is therefore still open; the EPD426-v1 diag is
  evidence for the sibling panel, not Murphy's.
- The Gitee repo is binary-only, so it isn't a source for the LUT
  contents — extraction-from-binary, as in
  `oem_touch_v525_grayscale_luts.md`, is the available route.

## Related findings

- `m3/findings/oem_touch_v525_grayscale_luts.md` — OEM LUT extraction
- `m4/findings/murphy_reader_code_reuse.md` — the separately-shipped
  Murphy Reader v1.2.16 (Murphy M4 / MurphyOS) at `murphy.pandacat.ai` is a *different* fork
  (built from `crosspoint-reader-main`, not directly from MoFei), and
  is the firmware that adds OPDS / KOReader Sync / Calibre features.
