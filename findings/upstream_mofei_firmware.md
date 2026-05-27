# Upstream Project: MoFei / corogoo EPD426

A second firmware blob (`~/Downloads/firmware.bin`, 5,626,704 bytes, MD5
`4821b423e69d72d09b43027a00ca34ba`, build `Mar  5 2024 12:12:53`, ESP-IDF
`v4.4.7-dirty`) was analyzed and turned out to be the **public open-source
project that Murphy M3's firmware is built from**, not a Murphy build.

## Identity

- App name in OTA UI: **MoFei** (墨非 / Pinyin for "Murphy")
- OTA update URL embedded in the binary:
  `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/EPD426-v1/update.json`
- mDNS host: `mofei.local`
- USB MSC volume label: `MOFEI Storage`
- On-device data directory: `/.mofei/` (epub cache, `read_history.json`,
  `lock_screen_pic.bin`)
- Build target: `EPD426-v1` (4.26-inch panel — same physical dimensions as
  Murphy's 240x416)
- Same ESP-IDF version, Arduino-lib-builder project name, and exact build
  timestamp as the Murphy app image extracted from the device. Murphy is
  almost certainly a rebrand/fork of this upstream, not an independent
  implementation.

## Why this matters for the RE work

Until now we have been treating Murphy as a closed binary and bench-tuning
panel behavior from scratch (see `display_lut_refresh.md`). The upstream is
public source on Gitee:

  https://gitee.com/corogoo/3.7-inch-ink-screen-reader

That source should contain:

- The actual UC8253 init sequence and LUTs (B/W fast, gray, ghost-clear)
- The DTM1/DTM2 plane-write conventions we reverse-engineered (which match
  what we found — destination-only, same buffer to both planes)
- The grayscale pipeline. The upstream binary has a built-in diagnostic
  page `Gray Refresh Test` / `Tap screen to draw black circle` /
  `Expected: black | dark gray | light gray | white` — meaning the OEM
  ships **working 4-level grayscale** on the same panel. Our conclusion
  in `display_lut_refresh.md` that "the panel can't do clean AA" should
  be revisited against the upstream LUT recipe before staying with the
  AA-disabled shipping state.
- The pin map, touch driver (FT6336 family), front-light PWM tables,
  audio and RTC code that we have separately reverse-engineered.

## Shared reader internals (string-level evidence)

Same log-tag conventions as Murphy:

- `[EPUB][reader] cache no-progress ...`
- `[EPUB][chapter] parse start tocIndex=...`
- `[EPUB][layout] marker hit pos=...`
- `[EPUB][drawTXT] pageStart=...`
- `[READER] page render %s: %lu ms`
- `[UI ] lines pageStart=... count=... y=`

These are identical format strings, in identical order, with the same
percent-format placeholders — so the EPUB/reader subsystem is shared
code.

## Diffs from the on-device Murphy build

The downloaded upstream blob is **smaller** (5.6 MB vs ~7.1 MB partition
on Murphy) and the MD5 differs. The most likely interpretation is that
Murphy is a downstream branch with extra customization (cover-art cache,
device-specific UI strings, possibly the touch-driver tweaks we already
ship in `crosspoint-reader-main`).

The Settings/UI strings show the upstream uses a web tab layout with
WiFi / Weather / FileManager / Universal-Input / FirmwareUpdate. Murphy
appears to have added device profile abstractions and a different
shell on top.

## Next steps if revisiting

1. Clone the Gitee repo and diff its display driver against our
   `community-sdk/libs/display/EInkDisplay` Murphy paths — especially:
   - UC8253 LUT tables (gray + B/W)
   - Booster (cmd `0x06`) and VCOM_DC (cmd `0x82`) values
   - Two-plane write convention
2. Re-run the grayscale tuning with upstream LUTs as the starting point
   before re-deciding whether to keep AA disabled on Murphy.
3. Check upstream commit history for OEM-side fixes (touch polarity,
   front-light curve) we may have independently reproduced.
