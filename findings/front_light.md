# Front Light Hardware And Firmware Notes

## High Confidence

The OEM firmware exposes a front-light setting.

On the physical Murphy unit, pressing and holding either right-side button opens the front-light controls. The top-right button and the lower button directly below it perform the same front-light shortcut. This should be treated as confirmed user-facing behavior for this hardware, not only a seller-listing claim.

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
- A top-right-button long press opens the backlight brightness setting menu. On the Murphy unit, the lower right-side button does the same thing.

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

An external report from another owner with the same display says the front-light LED strip is wired in parallel, is intended for 3.3 V drive, and has only one light channel. That rules out warm/cool dual-channel blending for this display family and makes single-channel PWM the leading control model.

The same report notes the useful electrical distinction: series LED strings, such as the 4.26-inch panel variant, need higher voltage but only draw roughly 15 mA; parallel LED strips need lower voltage but higher current. That matches the Good Display 3.7-inch front-light spec: low operating voltage, but a maximum current high enough that the board-side current path still matters.

The public CrowPanel 3.7-inch Arduino `3.7_TF` example labels GPIO42 as the screen backlight/front-light pin and sets it high during setup. Earlier notes treated GPIO42 as an SD-card power enable, but that is now considered incorrect for the Murphy/CrowPanel lineage. The current SDK board profile should reserve GPIO42 for front-light investigation, not SD power.

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
- A single-channel 3.3 V LED strip with parallel LEDs.
- Low-voltage, higher-current behavior compared with higher-voltage series LED strings.
- A board-side PWM path driving the LED rail or a driver/transistor input.
- User-facing brightness controlled through a right-side-button long-press UI, with approximately 10 levels matching the seller listing.

Use PWM above the audible range. A practical target is at least 20 kHz; 25 kHz is a good default if the LEDC resolution is still adequate for 10 brightness levels.

The Good Display panel lead says the light assembly itself is low-voltage and up to 60 mA. That may be fine for a 3.3 V LED rail, but it is too much to source directly from an ESP32-S3 GPIO. Until measured otherwise, assume the MCU GPIO should drive a MOSFET/transistor gate or LED-driver control input rather than the LED current directly.

## Unknowns

| Item | Status |
| --- | --- |
| Front-light GPIO pin | Unknown |
| PWM channel/timer | Unknown |
| PWM frequency/resolution | Likely single-channel PWM; use at least 20 kHz. Exact OEM value unknown. |
| Active polarity | Unknown |
| Whether there is a separate enable pin | Unknown; may be PWM-only. |
| Whether there is a dedicated LED-driver IC | Unknown |
| Brightness persistence key/NVS field | Unknown |

## Porting Impact

For `community-sdk`, front light should be a board-level peripheral, not part of the UC8253 display driver:

- Add or reserve a `HalFrontLight` abstraction behind `BoardConfig::hasFrontLight`.
- Expose `setLevel(0..10)` and `getLevel()`.
- Keep the implementation disabled until pin/control-path proof exists.
- Once verified, implement the Murphy backend with configurable PWM pin, optional enable pin, PWM frequency, duty table, and active polarity.
- Use a PWM frequency above the audible range, preferably around 25 kHz, to avoid coil/capacitor/ceramic whine.
- Wire CrossPoint's lighting UI to the abstraction after the board backend is proven.
- Preserve the OEM shortcut: long-pressing either right-side button should open front-light controls.

Do not assume the front light shares touch or display pins. Treat it as its own power/control circuit.

## Next Reverse-Engineering Steps

1. In Ghidra, continue from the `Front Light` string-pointer table and identify the settings-dispatch table that consumes it.
2. Search decompiled code for callers of Arduino `ledcSetup` / `ledcWrite` and ESP-IDF `ledc_*` functions, then recover constant GPIO/channel values.
3. Search NVS/default-setting code for a small integer stored near other settings such as dark mode, refresh interval, or font size.
4. Probe the Murphy PCB around the panel/front-light FPC for LED-driver IC markings or MOSFET/transistor routing.
5. With the OEM firmware running, use a logic analyzer, scope, or meter on likely LED-control pins while changing brightness through the right-side-button long-press front-light controls. OEM USB logs are optional; pin/rail behavior is the source of truth.
6. Measure the front-light rail voltage/current at several brightness levels to determine actual current draw and whether direct GPIO drive is electrically safe or a transistor/driver is required.
7. If stock-firmware probing is not possible, run a custom front-light sweep firmware that PWM-tests candidate GPIOs one at a time at 25 kHz with conservative duty limits, watching for visible light changes and avoiding any display/touch pins already under test.
