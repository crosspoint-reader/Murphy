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

The public CrowPanel 3.7-inch Arduino `3.7_TF` example labels GPIO42 as the screen backlight/front-light pin and sets it high during setup. That lead did not hold for this Murphy unit: custom PWM tests on GPIO42 and GPIO41 did not turn the light on. The confirmed Murphy front-light control pin is GPIO48.

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

## Custom Firmware Proof

The standalone CrossPoint probe `murphy_frontlight_probe_v3_gpio48_pwm` confirmed the Murphy front light on `GPIO48`.

Observed behavior:

- `GPIO48` digital low turns the light off.
- `GPIO48` digital high turns the light on.
- LEDC PWM at 25 kHz changes brightness smoothly.
- The probe used 10-bit duty values from `0..1023`.
- Releasing `GPIO48` to input returns the board to a passive state.

Negative controls:

- GPIO42 did not turn the light on during the PWM probe.
- GPIO41 did not turn the light on during the PWM probe.
- Earlier `INPUT_PULLUP` testing on GPIO48 turned or latched the front light on, which explains why broad I2C/IRQ sweeps involving GPIO48 produced misleading results.

Porting consequence: `GPIO48` must be reserved for front-light control on Murphy M3. Do not use it as display `BUSY`, touch I2C, RTC I2C, or a generic input candidate.

## Current Working Model

The likely hardware shape is:

- A front-light LED rail integrated into the panel assembly.
- A single-channel 3.3 V LED strip with parallel LEDs.
- Low-voltage, higher-current behavior compared with higher-voltage series LED strings.
- A board-side active-high PWM path on `GPIO48`, likely driving the LED rail through a transistor/driver input.
- User-facing brightness controlled through a right-side-button long-press UI, with approximately 10 levels matching the seller listing.

Use PWM above the audible range. A practical target is at least 20 kHz; 25 kHz is a good default if the LEDC resolution is still adequate for 10 brightness levels.

The Good Display panel lead says the light assembly itself is low-voltage and up to 60 mA. That may be fine for a 3.3 V LED rail, but it is too much to source directly from an ESP32-S3 GPIO. Until measured otherwise, assume the MCU GPIO should drive a MOSFET/transistor gate or LED-driver control input rather than the LED current directly.

## Unknowns

| Item | Status |
| --- | --- |
| Front-light GPIO pin | Confirmed `GPIO48` |
| PWM channel/timer | Any free LEDC channel/timer should work; OEM channel unknown |
| PWM frequency/resolution | 25 kHz / 10-bit verified in custom firmware; OEM value unknown |
| Active polarity | Active high |
| Whether there is a separate enable pin | No separate enable identified; current evidence supports PWM-only control on GPIO48 |
| Whether there is a dedicated LED-driver IC | Unknown |
| Brightness persistence key/NVS field | Unknown |

## Porting Impact

For `community-sdk`, front light should be a board-level peripheral, not part of the UC8253 display driver:

- Add or reserve a `HalFrontLight` abstraction behind `BoardConfig::hasFrontLight`.
- Expose `setLevel(0..10)` and `getLevel()`.
- Implement the Murphy backend with `GPIO48`, active-high PWM, 25 kHz default frequency, and a 10-level duty table.
- Use a PWM frequency above the audible range, preferably around 25 kHz, to avoid coil/capacitor/ceramic whine.
- Wire CrossPoint's lighting UI to the abstraction after the board backend is proven.
- Preserve the OEM shortcut: long-pressing either right-side button should open front-light controls.

Do not assume the front light shares touch or display pins. Treat it as its own power/control circuit.

## Next Reverse-Engineering Steps

1. Implement `GPIO48` active-high PWM in `community-sdk` as the Murphy front-light backend.
2. Choose a conservative 10-level duty table and validate the perceived brightness curve on hardware.
3. In Ghidra, continue from the `Front Light` string-pointer table and identify the settings-dispatch table that consumes it.
4. Search NVS/default-setting code for a small integer stored near other settings such as dark mode, refresh interval, or font size.
5. If the device can ever be opened, measure the front-light rail voltage/current at several brightness levels to determine the actual current path and driver margin.
