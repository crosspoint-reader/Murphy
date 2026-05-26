# Display Bring-Up Notes

## Current Status

The Murphy M3 CrossPoint bring-up can boot ESP32-S3 firmware and now visibly drives the e-paper panel from custom firmware. The working hardware path is the OEM-derived GPIO3-8 bit-banged UC8253 bus; the remaining work is refresh quality, LUT/plane handling, and app/UI rendering polish.

Confirmed panel family:

- Good Display `GDEY037T03-FT21`
- 3.7-inch e-paper with touch and front light
- UC8253 controller
- 416x240 resolution
- SPI display interface
- 24-pin display FPC
- 6-pin FT6336U touch FPC
- 6-pin front-light FPC

Physical panel/FPC marking observed on the Murphy display:

- `YSFK082048A-W-3`
- `LH37004D 2513`
- small FPC label text appears to begin with `FPC F...`

No useful public indexed match was found for `YSFK082048A-W-3` or `LH37004D` during a web search. Treat these as module/FPC production markings for the `GDEY037T03-FT21` assembly, not necessarily as public product numbers.

The public CrowPanel 3.7-inch Arduino examples define:

| Signal | Primary GPIO |
| --- | ---: |
| EPD SCK | 12 |
| EPD MOSI | 11 |
| EPD CS | 45 |
| EPD DC | 46 |
| EPD RST | 47 |
| EPD BUSY | 48 |
| Display power enable | 7 |
| Front light/backlight | 42 |

Important Murphy-specific correction: custom front-light probing has now confirmed `GPIO48` as the active-high PWM front-light control pin on this Murphy unit. The public CrowPanel `EPD BUSY = 48` mapping is therefore invalid for this hardware. Treat any earlier `GPIO48` BUSY readings, and any I2C/touch false positives involving GPIO48, as artifacts of driving the front-light control line.

New Murphy-specific display pin lead: a focused GPIO3-8 probe produced real BUSY activity with this map:

| Function | GPIO |
| --- | ---: |
| EPD MOSI/SDA | 3 |
| EPD SCK/CLK | 4 |
| EPD CS | 5 |
| EPD DC | 6 |
| EPD RST | 7 |
| EPD BUSY | 8, ready-high |

Observed behavior from `murphy_epd_gpio3_8_probe_v1`: initial BUSY was low, reset made BUSY high, and `0x04` power-on, `0x12` refresh, and `0x02` power-off each produced a low-to-high BUSY transition within roughly 10-50 ms. Later CrossPoint builds on this same tuple rendered the boot/home UI, confirming it as the actual Murphy EPD control path. Current display issues are not broad pin-map issues; they are refresh/waveform/frame-plane behavior on the GPIO3-8 map.

See `display_gpio_recovery.md` for the OEM constructor/decompile evidence and the recovered OEM init sequence:
`0x01: 03 10 3F 3B 0D`, `0x06: D7 D7 1F`, `0x04`, `0x00: FF`, `0x30: 09`, `0x61: F0 01 A0`, `0x82: 0F`, `0x50: 97`.

The same examples also contain an alternate commented control-pin set:

| Signal | Alternate GPIO |
| --- | ---: |
| EPD CS | 10 |
| EPD DC | 9 |
| EPD RST | 21 |
| EPD BUSY | 48 |

The Good Display sample archive from `/Users/jmitch/Downloads/A32-GDEY037T03-FT21.rar` was extracted under `analysis/vendor/gooddisplay/`. Its Arduino sample for `TY037T03` uses a different adapter-board mapping:

| Signal | Good Display sample GPIO assumption |
| --- | ---: |
| EPD SCK | 23 |
| EPD MOSI | 18 |
| EPD BUSY | A14, likely GPIO14 on that sample board |
| EPD RST | A15, likely GPIO15 |
| EPD DC | A16, likely GPIO16 |
| EPD CS | A17, likely GPIO17 |
| FT6336 IRQ | A0 |
| FT6336 RST | A3 |
| FT6336 SDA | A4 |
| FT6336 SCL | A5 |

The sample comments say `SCLK--GPIO23` and `MOSI---GPIO18`, but that mapping is for Good Display's sample ESP32 adapter, not the ESP32-S3 target used here. `GPIO23` is invalid on this ESP32-S3 PlatformIO target, so the active standalone probe does not touch it.

Under the current `esp32-s3-devkitc1-n16r8` Arduino variant, the analog aliases are:

| Alias | ESP32-S3 GPIO |
| --- | ---: |
| A14 | 15 |
| A15 | 16 |
| A16 | 17 |
| A17 | 18 |

So the active Good Display-derived probe maps the sample as `BUSY15/RST16/DC17/CS18` and uses the S3 default SPI pins `SCK12/MOSI11`.

A second archive, `/Users/jmitch/Downloads/A32-GDEY037T03.rar`, was extracted to `analysis/vendor/gooddisplay/A32-GDEY037T03/`. It is labeled `A32-GDEY037T03-FP4G-20240815` and appears to target the non-FT base panel / 4-gray sample rather than the `FT21` touch/front-light assembly. It uses the same `A14/A15/A16/A17` control-pin aliases, so it does not add a new Murphy board pin map.

The Waveshare `ESP32-S3-ePaper-3.97` repo is for a different 3.97-inch 800x480 panel, so its display command set is not directly portable to the Murphy 3.7-inch UC8253 panel. Its ESP32-S3 board pin map is still a useful clue because it uses another e-paper S3 layout:

| Signal | Waveshare ESP32-S3-ePaper-3.97 GPIO |
| --- | ---: |
| EPD SCK | 11 |
| EPD MOSI | 12 |
| EPD CS | 10 |
| EPD DC | 9 |
| EPD RST | 46 |
| EPD BUSY | 3 |

This `SCK11/MOSI12/CS10/DC9/RST46/BUSY3` mapping is now included in the standalone probe with UC8253-style init sequences. Treat it as a board-family lead only; the Waveshare display controller sequence itself is for an 800x480 panel and waits for BUSY low-idle rather than the Good Display UC8253 high-idle convention.

The Good Display init sequence also differs from the CrowPanel-style code:

| Setting | CrowPanel-style probe | FT21 sample | 2024 base-panel sample |
| --- | ---: | ---: | ---: |
| Power-on timing | before refresh | during init |
| Full init `0x50` | `0xD7` | `0x97` | `0x97` |
| Fast init `0xE5` | `0x5F` | `0x32` | `0x5F` |
| Fast init `0x50` | `0xD7` | `0x97` | not set in fast init |
| 4-gray init `0xE5` | not tested | not present in FT21 sample path | `0x5A` |
| BUSY polarity | wait for high | wait for high | wait for high |

## OEM LUT Evidence

The OEM app does contain LUT-related strings:

- `LUT_DEL`
- `/lut.bin`
- `/lut_full.bin`
- `LUT_ADD`
- `LUT_ADD OK`
- `LUT_FULL_ADD`
- `LUT_FULL_ADD OK`
- `Not full refresh`

This means the stock firmware has some path for adding/removing partial and full refresh LUT files. It does not, by itself, mean the flashed unit contains those LUT files.

The dumped `spiffs` partition is erased/all `0xff`, and no `/lut.bin` or `/lut_full.bin` contents were recovered from the filesystem. Headless Ghidra pointer-reference mining found the LUT strings in an app literal/table region around `0x42000c34`, but not a clean display-driver function or embedded LUT blob. Exact byte searches for the public 31-byte UC8253 LUTs and several public Good Display/CrowPanel init byte sequences did not match `analysis/extracted/app0.bin`.

Current conclusion: the OEM binary proves that LUT file support exists, but this dump does not currently provide recoverable OEM LUT bytes. The standalone probe therefore now includes public UC8253 LUT-write modes as hypotheses, not as confirmed Murphy OEM waveforms.

## Static RE Pin-Map Search

Static reverse engineering has produced clues, but not a definitive display GPIO map.

Useful OEM clues:

- The app embeds the update/source URL `https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json`.
- The matching public Gitee repo exists as `corogoo/3.7-inch-ink-screen-reader`, described as a 3.7-inch ink-screen reader repo, but the visible tree is firmware-oriented and does not expose a clear source-level display pin map.
- OEM strings prove app-level display/LUT features: `displayOtaTask`, `Not full refresh`, `LUT_ADD`, `LUT_FULL_ADD`, `/lut.bin`, `/lut_full.bin`, `Refresh`, and `EPD BS`.

Negative RE results so far:

- Headless Ghidra callsite mining for the display/LUT strings mostly lands in literal tables or bad/offcut code, not clean OEM display functions.
- Exact raw-image searches did not find simple byte or uint32 pin tuples for the public CrowPanel map, alternate CrowPanel map, Good Display sample map, Waveshare map, or the currently pruned Good Display-style candidates.
- The dumped filesystem is erased, so `/lut.bin` and `/lut_full.bin` are not recoverable from this unit's `spiffs` partition.

Interpretation: the OEM app likely computes or inlines the display pin setup through Arduino/IDF wrappers, or Ghidra is not recovering the stripped Xtensa call graph well enough to bind GPIO literals to the display functions. RE is still useful for ruling things out and finding external provenance, but it has not yet yielded a trustworthy display pin tuple.

## Custom Firmware Tests

A full CrossPoint build and a standalone `murphy_display_probe` PlatformIO environment were tested.

Observed with the standalone probe:

- GPIO7 is driven high.
- GPIO42 was driven high in early public-reference tests, but later dedicated front-light probing showed GPIO42 is not the Murphy front-light control pin.
- SCK/MOSI/CS/DC/RST pins toggle as expected.
- GPIO48 was initially observed as a stuck-low public-reference BUSY candidate, but it is now confirmed as the front-light PWM pin and must be removed from display BUSY hypotheses.
- Good Display sample `A14/A15/A16/A17` aliases were translated through the active ESP32-S3 Arduino variant as `BUSY15/RST16/DC17/CS18`. In probe runs, GPIO15 stayed high before reset, during reset, during power-on, during refresh, and during power-off.
- The raw numbered `BUSY14/RST15/DC16/CS17` sanity case also stayed high throughout.
- The 2024 `A32-GDEY037T03-FP4G` full and fast init sequences were tested on the same A-alias pins. They produced the same behavior: GPIO15 stayed high and no visible display update occurred.
- The Waveshare ESP32-S3 e-paper board-family mapping `SCK11/MOSI12/CS10/DC9/RST46/BUSY3` was tested with CrowPanel, Good Display 2024 full, and FT21 fast UC8253-style init modes. GPIO3 stayed low throughout reset, power-on, refresh, and power-off, matching the no-response behavior seen on GPIO48.
- No visible panel update occurs, even with a forced full-screen stripe pattern.

Probe matrix:

- Good Display sample A-alias control pins with S3 default SPI: `SCK12 MOSI11 CS18 DC17 RST16 BUSY15`.
- Good Display sample raw-numbered sanity check: `SCK12 MOSI11 CS17 DC16 RST15 BUSY14`.
- Good Display 2024 base-panel full init and fast init are now included as standalone probe modes on the A-alias pins.
- Waveshare ESP32-S3 e-paper board-family pin lead: `SCK11 MOSI12 CS10 DC9 RST46 BUSY3`.
- Primary public-reference control pins: `CS45 DC46 RST47 BUSY48`, now invalid as a full Murphy display tuple because `GPIO48` is front-light PWM.
- Alternate public-reference control pins: `CS10 DC9 RST21 BUSY48`, now invalid as a full Murphy display tuple because `GPIO48` is front-light PWM.
- GPIO7 tested high, low, and input.
- GPIO42 tested high and low.
- Several obvious black/white patterns were written.

Result: no tested firmware-side combination changed the display or caused the candidate BUSY line to show controller activity. `GPIO48` is no longer a valid BUSY candidate because it controls the front light. GPIO3 stayed low in the Waveshare-style cases; GPIO14/GPIO15 stayed high in the Good Display-derived cases, including the 2024 base-panel sample sequences. Early probe versions only waited for BUSY-high readiness; the expanded blind probe now also tests BUSY-low readiness and an ignore-BUSY fixed-delay mode.

Update: the GPIO3-8 map above supersedes the older Waveshare-style interpretation that used GPIO3 only as BUSY. Under the new map, GPIO3 is MOSI and GPIO8 is BUSY. The successful BUSY transitions on GPIO8 make this the active display hypothesis.

## Interpretation

The current evidence does not support treating the public CrowPanel board-level GPIO mapping as confirmed for this Murphy M3 unit. The Murphy display FPC/control pins differ from the public CrowPanel example, and `GPIO48` must not be considered for BUSY because it is confirmed front-light PWM.

The correct path is the OEM-derived GPIO3-8 map:

```text
MOSI=GPIO3
SCK=GPIO4
CS=GPIO5
DC=GPIO6
RST=GPIO7
BUSY=GPIO8 ready-high
```

Custom firmware has rendered the CrossPoint boot/home UI with this path. Any remaining visible failures should be debugged as UC8253 init/LUT/plane/refresh problems, not as a reason to return to broad pin sweeps.

## Next Required Evidence

Further blind firmware permutations are low value. OEM USB logs are not required here. If the device could be opened, the useful evidence would be electrical signal capture or voltage probing while the stock OEM firmware performs a real refresh:

1. Restore or boot the OEM firmware.
2. Attach a logic analyzer to likely display lines around the ESP32-S3 or panel FPC.
3. Trigger a known OEM screen refresh.
4. Capture SCK, MOSI, CS, DC, RST, BUSY candidates, GPIO7, and GPIO48 front-light PWM.
5. Confirm which GPIOs actually toggle and where BUSY is routed. Do not use GPIO48 as BUSY on Murphy.

At minimum, capture these candidate lines:

- `GPIO12`, `GPIO11`, `GPIO45`, `GPIO46`, `GPIO47`
- `GPIO10`, `GPIO9`, `GPIO21`
- `GPIO18`, `GPIO17`, `GPIO16`, `GPIO15`, `GPIO14`
- `GPIO7`, `GPIO42`, `GPIO48`

If a logic analyzer is unavailable, use a meter or scope first to confirm whether GPIO7 actually powers the display rail and to look for BUSY activity on non-GPIO48 candidates. GPIO48 should instead be checked only as the front-light PWM/control signal.

Current practical constraint: this unit cannot be opened, so internal signal capture and pad probing are not available.

No-log/no-open fallback order:

1. Continue static OEM analysis for app-level display init and GPIO constants.
2. Use USB-flashed custom firmware as the only active test surface.
3. Test bounded display hypotheses from public references and static leads only.
4. Use visible screen change, not BUSY alone, as the success signal.
5. Move any visibly confirmed pin set back into the standalone `murphy_display_probe` and then the SDK board profile.

This is better than relying on OEM log text because the display failure is below the UI/logging layer.

See `findings/non_invasive_plan.md` for the current no-log/no-open recovery plan.

## Expanded Blind Probe

The standalone CrossPoint probe at `/Users/jmitch/GitHub/crosspoint-reader-main/src/murphy_display_probe.cpp` has been expanded for the no-log/no-open case.

It now generates 1440 bounded cases:

- 2 active display pin hypotheses after pruning noisy public-primary candidates.
- 8 power-enable combinations across GPIO7, GPIO42, and GPIO41.
- 3 BUSY strategies: ready-high, ready-low, and ignore-BUSY fixed delays.
- 6 UC8253 init/LUT sequences.
- 5 frame-write strategies.
- Rotating visible patterns: vertical stripes, horizontal bands, block pattern, full black.

Display pin hypotheses:

| Name | SCK | MOSI | CS | DC | RST | BUSY |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `no_primary_gd_raw_mosi18` | 12 | 18 | 17 | 16 | 15 | 14 |
| `no_primary_gd_raw_sck18_mosi12` | 18 | 12 | 17 | 16 | 15 | 14 |

Pruned candidates remain in source as named constants for historical context, but they are no longer in the active matrix. The click probe made GPIO11, GPIO45, and GPIO46 suspect: GPIO11 clicked when toggled as MOSI with GPIO7 low, GPIO45 clicked when toggled as CS with GPIO7 high, and GPIO46 clicked when driven/toggled as DC. The active matrix now avoids those three GPIOs.

Power-enable combinations:

| Name | GPIO7 | GPIO42 | GPIO41 |
| --- | ---: | ---: | ---: |
| `all_input` | input | input | input |
| `pwr7_high` | high | input | input |
| `fl42_high` | input | high | input |
| `led41_high` | input | input | high |
| `pwr7_fl42_high` | high | high | input |
| `pwr7_led41_high` | high | input | high |
| `fl42_led41_high` | input | high | high |
| `all_high` | high | high | high |

BUSY strategies:

| Name | Behavior |
| --- | --- |
| `ready_high` | Wait until BUSY reads high, matching the earlier CrowPanel-style assumption. |
| `ready_low` | Wait until BUSY reads low, covering the X3-style inverted readiness case. |
| `ignore_busy` | Do not trust BUSY; use fixed delays for reset, init, refresh, and power-off. |

Init/LUT strategies:

| Name | Behavior |
| --- | --- |
| `crowpanel` | Public CrowPanel-style fast init plus power-on before refresh. |
| `ft21_fast` | Good Display FT21 sample fast init: power-on, `0xE0=0x02`, `0xE5=0x32`, `0x50=0x97`. |
| `gd2024_full` | 2024 base-panel full init: power-on, `0x50=0x97`. |
| `gd2024_fast` | 2024 base-panel fast init: power-on, `0xE0=0x02`, `0xE5=0x5F`. |
| `crowpanel_lut_full31` | CrowPanel-style fast init followed by the public 31-byte UC8253 full-refresh LUT via command `0x32`. |
| `crowpanel_lut_part31` | CrowPanel-style fast init followed by the public 31-byte UC8253 partial-refresh LUT via command `0x32`. |

The probe now matches the public CrowPanel update order more closely: it writes the old/new image RAM buffers first, then sends `0x04` power-on immediately before `0x12` display refresh. Earlier versions could power on before the frame write, which can still produce audible panel drive without necessarily applying the intended RAM contents.

Frame-write strategies:

| Name | Behavior |
| --- | --- |
| `vendor_10_13` | Public CrowPanel-style old/new RAM write using `0x10` then `0x13`. |
| `window_10_13` | Explicitly sets RAM data entry, X/Y window, and X/Y counters before `0x10`/`0x13`. |
| `single_13` | Writes only the new-image buffer through `0x13`. |
| `single_24` | Writes the image through common single-buffer command `0x24`. |
| `window_invert_10_13` | Explicit RAM window plus inverted byte polarity through `0x10`/`0x13`. |

Inactive candidate pins are released to input before each case. The probe does not do an arbitrary all-GPIO sweep; it only tests pin sets with some public-reference or static-analysis basis.

Build command:

```sh
cd /Users/jmitch/GitHub/crosspoint-reader-main
pio run -e murphy_display_probe
```

Upload/monitor command:

```sh
pio run -e murphy_display_probe -t upload
pio device monitor -e murphy_display_probe -b 115200
```

If the screen changes, note the latest serial line beginning with:

```text
OBSERVE NOW: if the screen changed, note case ...
```

That line contains the case number, pin set, power combination, and init mode needed to promote the result into the SDK.

## Click Isolation

A second standalone probe exists at `/Users/jmitch/GitHub/crosspoint-reader-main/src/murphy_click_probe.cpp`.

Use it when every display matrix case produces the same audible click. If every case clicks, the click is not strong evidence that the correct display command path was found; it may be caused by a power rail, reset line, speaker/audio path, front-light circuit, or generic high-voltage/PMIC behavior that the matrix repeatedly triggers.

Build/upload:

```sh
cd /Users/jmitch/GitHub/crosspoint-reader-main
pio run -e murphy_click_probe -t upload
pio device monitor -e murphy_click_probe -b 115200
```

The probe prints `LISTEN:` labels and pauses between phases. The important result is the first label where the click occurs, for example GPIO7 high/low, reset pulse, SPI-like clocks, or minimal refresh command sequence.

The current click evidence points at GPIO state/rail transitions rather than a confirmed e-paper refresh. Clicks were observed during SPI-like activity with GPIO7 high, during DC-only toggling with GPIO7 high, when GPIO7 was driven low after per-signal toggles, and during a later bus idle configuration step with no explicit power enable. The click probe now includes pin-numbered per-signal isolation labels, GPIO7-low/GPIO7-high comparisons, and slow/fast DC toggles. Use those labels to determine whether the audible noise is tied to a specific GPIO level, edge rate, or rail transition.

A click was observed at `DC set OUTPUT HIGH, no power enables` on the primary map, where DC is `GPIO46`. Since `GPIO46` is also the Waveshare-style reset line, the display matrix now includes hybrid cases that treat `GPIO46` as RST and `GPIO9` as DC while keeping `SCK12/MOSI11` and trying both primary/alternate CS and BUSY leads.
