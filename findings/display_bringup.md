# Display Bring-Up Notes

## Current Status

The Murphy M3 CrossPoint bring-up can boot ESP32-S3 firmware and run display code, but the e-paper panel has not visibly changed under custom firmware yet.

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

The same examples also contain an alternate commented control-pin set:

| Signal | Alternate GPIO |
| --- | ---: |
| EPD CS | 10 |
| EPD DC | 9 |
| EPD RST | 21 |
| EPD BUSY | 48 |

## Custom Firmware Tests

A full CrossPoint build and a standalone `murphy_display_probe` PlatformIO environment were tested.

Observed with the standalone probe:

- GPIO7 is driven high.
- GPIO42 is driven high.
- SCK/MOSI/CS/DC/RST pins toggle as expected.
- GPIO48 remains low before reset, during reset, after reset, during UC8253 init, during power-on, during refresh, and during power-off.
- No visible panel update occurs, even with a forced full-screen stripe pattern.

Probe matrix:

- Primary control pins: `CS45 DC46 RST47 BUSY48`.
- Alternate control pins: `CS10 DC9 RST21 BUSY48`.
- GPIO7 tested high, low, and input.
- GPIO42 tested high and low.
- Several obvious black/white patterns were written.

Result: no tested firmware-side combination changed the display or caused BUSY to leave low.

## Interpretation

The current evidence does not support treating the public CrowPanel display pin mapping as confirmed for this Murphy M3 unit. The panel may still be UC8253-compatible, but one or more of the following is likely true:

- The Murphy display FPC/control pins differ from the public CrowPanel example.
- GPIO48 is not the actual BUSY signal on this unit.
- An additional display power/enable rail is required.
- The panel is behind a different board-level buffer, gate, or PMIC sequence.
- The display controller or module variant differs from the assumed public CrowPanel/Good Display module.

This failure is below CrossPoint UI/rendering. It occurs with a standalone firmware that only bit-bangs the vendor UC8253 sequence.

## Next Required Evidence

Further blind firmware permutations are low value. The next useful step is signal capture from the stock OEM firmware:

1. Restore or boot the OEM firmware.
2. Attach a logic analyzer to likely display lines around the ESP32-S3 or panel FPC.
3. Trigger a known OEM screen refresh.
4. Capture SCK, MOSI, CS, DC, RST, BUSY, GPIO7, and GPIO42.
5. Confirm which GPIOs actually toggle and whether BUSY is active-low on GPIO48 or routed elsewhere.

At minimum, capture these candidate lines:

- `GPIO12`, `GPIO11`, `GPIO45`, `GPIO46`, `GPIO47`, `GPIO48`
- `GPIO10`, `GPIO9`, `GPIO21`
- `GPIO7`, `GPIO42`

If a logic analyzer is unavailable, use a meter or scope first to confirm whether GPIO7 actually powers the display rail and whether GPIO48 is hard-low, floating, or active during stock refresh.
