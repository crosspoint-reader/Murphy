# CrossPoint port: implementation notes

## Goal

Run open-source CrossPoint firmware on Murphy M4 hardware while retaining a
reliable path back to the device's own factory installation. Touch-controller
firmware modification is outside the scope of this project.

## Why the port was practical

The MurphyOS v1.2.16 OTA image contains substantial CrossPoint-derived code and
identifiable CrossPoint strings. The application architecture, ESP32-S3 target,
SSD1677 display support, and SDMMC support therefore had close open-source
counterparts. The work centered on establishing the M4 board profile and the
few hardware-specific paths that differ.

## Implemented changes

### FreeInk SDK

- Added the `FREEINK_DEVICE_MURPHY_M4` target and `MurphyM4` board profile.
- Configured the 800×480 framebuffer used for the physically portrait-mounted
  480×800 GDEQ0426T82/SSD1677 panel.
- Added the three-button GPIO map and 4-bit SDMMC wiring.
- Added support for the factory-labelled FT6336U-compatible touch protocol.
- Added dual-channel, 8-bit frontlight control with a perceptual PWM curve.
- Guarded sensor-bus initialization for boards without a configured sensor bus.

### CrossPoint application

- Added the `murphy_m4` PlatformIO environment.
- Added M4 device identification in the HAL.
- Added persistent frontlight state, brightness, and warmth settings.
- Added a touch-operated vertical frontlight panel.
- Added the global top+middle-button light toggle.
- Turned the frontlight off when entering sleep.
- Configured battery monitoring on GPIO9 with a 2:1 divider.
- Kept dynamic CPU-frequency switching disabled on M4 after touch instability
  was observed during validation.

## Confirmed user controls

| Input | Result |
|---|---|
| Tap left/right reader area | Previous/next page according to CrossPoint mapping |
| Swipe down from top edge | Open frontlight panel |
| Swipe up from bottom edge | Return home |
| Swipe right from left edge | Go back |
| Top + middle side buttons | Toggle frontlight globally |
| Frontlight panel: top/middle | Increase/decrease selected control |
| Frontlight panel: bottom short press | Switch brightness/warmth selection |

## Validation completed

- Application builds successfully with `pio run -e murphy_m4`.
- Application-only flashing at `0x10000` boots on a retail M4.
- The final reviewed build was flashed, hash-verified, and tested on hardware.
- Display refresh, navigation, touch keyboard accuracy, all three side buttons,
  SD-card book access, battery percentage, frontlight touch controls, the global
  frontlight shortcut, settings persistence, and touch after sleep/wake passed.
- Wi-Fi and normal reader operation were also exercised successfully.
- Battery ADC measured approximately 4.19 V while charged and displayed 100%.
- Touch coordinates were corrected to `swapXY=true`, `flipX=false`,
  `flipY=true` and verified with the on-screen keyboard.

## Remaining work

- Longer battery-life and suspend/resume testing.
- Testing on additional M4 units and hardware revisions.
- Confirming the touch-controller package marking independently of factory
  firmware strings.
- Separating and reviewing the FreeInk SDK and CrossPoint changes upstream.
- Release automation and a user-friendly installer after broader validation.

For build and recovery commands, see [crosspoint_port.md](crosspoint_port.md).
For the touch implementation and its non-update safety boundary, see
[touch.md](touch.md).
