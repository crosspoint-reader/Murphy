# Front Light Hardware And Firmware Notes

## High Confidence

The OEM firmware exposes a front-light setting.

Recovered strings from `analysis/extracted/app0.bin`:

| String | Evidence |
| --- | --- |
| `Front Light` | User-facing settings label at string offset `0x33a9`. |
| `Short press toggle, long press modify` | Nearby settings/help text, likely reused for adjustable settings. |
| `Hold up/down keys to adjust faster` | Nearby settings/help text for numeric adjustment UI. |

The `Front Light` label sits in the same settings-string cluster as `Full Ref Interval`, `Network`, `Clock Ref Interval`, `Time Format 24H`, `Weather No Night Update`, and `Dark Mode`, so this is application UI, not a generic library artifact.

Ghidra exports:

- `analysis/frontlight_string_refs.md`
- `analysis/frontlight_pointer_refs.md`
- `analysis/frontlight_table_memory.md`

The pointer/reference pass shows `Front Light` in a contiguous string-pointer table at `0x3fc9a9d4` / raw `0x00117da4`. No clean function reference to the front-light handler has been recovered yet.

## External Hardware Lead

The HamGeek M3 product listing claims:

- 10-level adjustable backlight/front light.
- A top-right-button long press opens the backlight brightness setting menu.

Source:

`https://www.hgeek.com/products/hamgeek-m3-portable-e-book-reader-3-7-inch-touch-screen-e-paper-10-level-adjustable-backlight-clock-e-ink-display`

Good Display's matching 3.7-inch touch/front-light panel family is the strongest public panel lead so far:

- `GDEY037T03-FT21`: 3.7-inch, 416x240, UC8253, touch screen, front light.
- Front-light connector is listed as a 6-pin FPC.
- Front-light LED count is listed as 9.
- Front-light operating voltage is listed as 2.8 V to 3.3 V.
- Front-light maximum current is listed as 60 mA.

Source:

`https://www.good-display.com/product/656.html`

This does not prove the Murphy M3 uses the exact `GDEY037T03-FT21` module, but it matches the known resolution, UC8253 family, touch feature, and front-light claim.

## Driver Path Evidence

The firmware includes Arduino-ESP32 and ESP-IDF LEDC/PWM code:

- `esp32-hal-ledc.c`
- `ledcSetup`
- `ledc_set_duty`
- `ledc_update_duty`
- `ledc_channel_config`
- `ledc_timer_config`

It also includes GPIO control code such as `gpio_set_level`.

Treat this as supporting evidence only. LEDC and GPIO functions are common in Arduino-ESP32 builds, and the current string/xref pass does not yet tie any LEDC channel or GPIO number directly to `Front Light`.

## Current Working Model

The likely hardware shape is:

- A front-light LED rail integrated into the panel assembly.
- A board-side enable/PWM path driving the LED rail.
- Either direct ESP32-S3 PWM into a transistor/driver, or a discrete LED driver with one or more control pins.
- User-facing brightness stored as approximately 10 levels, matching the seller listing.

The Good Display panel lead says the light assembly itself is low-voltage and up to 60 mA, which is too much for a bare ESP32 GPIO. The MCU should be assumed to drive a control input, not the LED string directly.

## Unknowns

| Item | Status |
| --- | --- |
| Front-light GPIO pin | Unknown |
| PWM channel/timer | Unknown |
| PWM frequency/resolution | Unknown |
| Active polarity | Unknown |
| Whether there is a separate enable pin | Unknown |
| Whether there is a dedicated LED-driver IC | Unknown |
| Brightness persistence key/NVS field | Unknown |

## Porting Impact

For `community-sdk`, front light should be a board-level peripheral, not part of the UC8253 display driver:

- Add or reserve a `HalFrontLight` abstraction behind `BoardConfig::hasFrontLight`.
- Expose `setLevel(0..10)` and `getLevel()`.
- Keep the implementation disabled until pin/control-path proof exists.
- Once verified, implement the Murphy backend with configurable PWM pin, optional enable pin, PWM frequency, duty table, and active polarity.
- Wire CrossPoint's lighting UI to the abstraction after the board backend is proven.

Do not assume the front light shares touch or display pins. Treat it as its own power/control circuit.

## Next Reverse-Engineering Steps

1. In Ghidra, continue from the `Front Light` string-pointer table and identify the settings-dispatch table that consumes it.
2. Search decompiled code for callers of Arduino `ledcSetup` / `ledcWrite` and ESP-IDF `ledc_*` functions, then recover constant GPIO/channel values.
3. Search NVS/default-setting code for a small integer stored near other settings such as dark mode, refresh interval, or font size.
4. Probe the Murphy PCB around the panel/front-light FPC for LED-driver IC markings or MOSFET/transistor routing.
5. With the OEM firmware running, use a logic analyzer on likely LED-control pins while changing brightness with the top-right-button long press flow.
6. Measure the front-light rail voltage/current at several brightness levels to determine whether brightness is PWM, current regulation, or stepped enable control.

