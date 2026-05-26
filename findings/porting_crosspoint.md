# Porting CrossPoint Reader To Murphy M3

## Executive Summary

Porting `/Users/jmitch/GitHub/community-sdk` and `/Users/jmitch/GitHub/crosspoint-reader-main` to the Murphy M3 is feasible, but it is not a drop-in board change. The major work is a new hardware profile for this ESP32-S3 e-paper board: display driver, pins, SD bus, input mapping, power enables, battery/charging behavior, partitions, and a UI pass for a much smaller 240x416 panel.

The original CrossPoint stack is built around Xteink X3/X4 devices on ESP32-C3. Murphy M3 appears to match the Elecrow CrowPanel ESP32 3.7-inch E-paper HMI family: ESP32-S3-WROOM-1-N8R8 class module, 8 MiB PSRAM, UC8253 e-paper controller, 240x416 black/white panel, SD slot, BLE/WiFi, two front buttons, and a rotary/key cluster. Our dumped unit has a 16 MiB flash image, so use the dump as truth for flash size even though the vendor README says 8 MB.

## Evidence Used

- Local firmware dump findings in this repo:
  - `findings/flash_layout.md`
  - `findings/firmware_identity.md`
  - `findings/hardware.md`
- CrossPoint app repo:
  - `/Users/jmitch/GitHub/crosspoint-reader-main/platformio.ini`
  - `/Users/jmitch/GitHub/crosspoint-reader-main/partitions.csv`
  - `/Users/jmitch/GitHub/crosspoint-reader-main/lib/hal/HalGPIO.h`
  - `/Users/jmitch/GitHub/crosspoint-reader-main/lib/hal/HalGPIO.cpp`
- Community SDK repo:
  - `/Users/jmitch/GitHub/community-sdk/libs/display/EInkDisplay`
  - `/Users/jmitch/GitHub/community-sdk/libs/hardware/InputManager`
  - `/Users/jmitch/GitHub/community-sdk/libs/hardware/SDCardManager`
  - `/Users/jmitch/GitHub/community-sdk/libs/hardware/BatteryMonitor`
- Vendor reference repo cloned to:
  - `analysis/vendor/CrowPanel-ESP32-3.7-E-paper-HMI-Display-with-240-416`

## Current CrossPoint Assumptions

`crosspoint-reader-main` currently builds as `board = esp32-c3-devkitm-1` with Arduino, PlatformIO, 16 MiB flash, OTA partitioning, and SDK libraries symlinked from `community-sdk`.

The HAL is Xteink-focused:

- Display SPI pins: SCLK 8, MOSI 10, CS 21, DC 4, RST 5, BUSY 6.
- One shared SPI MISO pin: GPIO7.
- Battery ADC: GPIO0.
- USB detect: UART0 RXD GPIO20.
- Input path: `InputManager`, which expects an ADC ladder on GPIO1/GPIO2 and a power button on GPIO3.
- Display library supports X4 SSD1677 at 800x480 and X3 UC81xx-class at 792x528.
- `EInkDisplay::MAX_BUFFER_SIZE` is sized for those larger panels, not a problem for Murphy, but the controller logic is not a direct UC8253 240x416 implementation.

## Murphy M3 / CrowPanel Board Profile

The vendor README and factory source identify:

- MCU: ESP32-S3-WROOM-1-N8R8 class.
- PSRAM: 8 MiB.
- Vendor README flash: 8 MB.
- Dumped firmware flash: 16 MiB. This should be treated as the actual target for this unit.
- Display controller: UC8253.
- Panel resolution: 240x416.
- Framebuffer size: `240 / 8 * 416 = 12480` bytes.
- Display color: black/white.
- Factory code uses BLE name `CrowPanel3-7`.

Relevant pins from vendor factory source and schematic:

| Function | GPIO | Notes |
| --- | ---: | --- |
| EPD SCK | 12 | Bit-banged in vendor examples |
| EPD MOSI/SDA | 11 | Display write path |
| EPD CS | 45 | Display chip select |
| EPD DC | 46 | Data/command |
| EPD reset | 47 | Panel reset |
| EPD busy | 48 | Busy is active low in vendor code |
| EPD power enable | 7 | Vendor sets high before display init |
| SD MOSI | 40 | Separate SD SPI bus |
| SD MISO | 13 | Separate SD SPI bus |
| SD SCK | 39 | Separate SD SPI bus |
| SD CS | 10 | Separate SD SPI bus |
| SD power enable | 42 | Vendor sets high |
| Power/status LED | 41 | Vendor sets high |
| Back / exit button | 1 | Digital active-low |
| Menu / home button | 2 | Digital active-low; also sleep wake source |
| Rotary previous/up | 6 | Digital active-low |
| Rotary next/down | 4 | Digital active-low |
| Rotary confirm | 5 | Digital active-low |

The schematic also exposes many GPIO header nets, but those are not required for a reader port.

## What Must Change In `community-sdk`

### 1. Add A Board Profile Layer

Do not overwrite the X3/X4 constants. Add a `MurphyM3` or `CrowPanel37S3` board profile with display pins, SD pins, input pins, power enables, panel dimensions, and feature flags.

Needed capabilities:

- `BOARD_MURPHY_M3` or `BOARD_CROWPANEL_37_S3`.
- `HAS_SEPARATE_SD_SPI = true`.
- `HAS_DISPLAY_POWER_ENABLE = true`.
- `HAS_SD_POWER_ENABLE = true`.
- `HAS_ROTARY_OR_5_KEY_INPUT = true`.
- `HAS_TOUCH = true`, but touch can be left unused for the first port.

### 2. Add A UC8253 240x416 Display Backend

Current `EInkDisplay` supports SSD1677 and an X3 UC81xx-class path, but Murphy needs the vendor UC8253 sequence:

- Reset, wait while busy is low.
- `0x00 0x1B` panel setting.
- `0xE0 0x02`.
- `0xE5 0x6E` for partial mode or `0x5F` for fast mode.
- `0x50 0xD7` border behavior.
- Write old frame to `0x10`.
- Write new frame to `0x13`.
- Power on with `0x04`, refresh with `0x12`.
- Power off with `0x02`, deep sleep via `0x07 0xA5`.

Minimum first target: black/white full-screen refresh only. Partial refresh and grayscale can come later.

The library should support runtime dimensions of 240x416. The existing static buffer can hold it, but the default `DISPLAY_WIDTH`/`DISPLAY_HEIGHT` assumptions and any X4/X3-only controller paths need to be guarded by board/panel type.

### 3. Split SD Onto Its Own SPI Bus

Current `SDCardManager` hardcodes:

- CS GPIO12.
- 40 MHz.
- Default `SdFat::begin(SD_CS, SPI_FQ)`.

Murphy uses a separate bus and power enable:

- `SD_SPI.begin(39, 13, 40, 10)`.
- `SD.begin(10, SD_SPI)` in vendor Arduino code, or equivalent SdFat configuration.
- GPIO42 high before SD init.

The SDK needs either constructor/config injection for SD pins and `SPIClass`, or a board-specific `SDCardManager` backend.

### 4. Replace The ADC-Ladder Input Manager

Current `InputManager` reads analog GPIO1/GPIO2 and power GPIO3. That conflicts with Murphy, where GPIO1 and GPIO2 are digital keys.

Add a digital input implementation for:

- Back: GPIO1.
- Menu/Home: GPIO2.
- Rotary previous/up: GPIO6.
- Rotary next/down: GPIO4.
- Rotary confirm: GPIO5.

Map these onto CrossPoint logical buttons:

- `Back` -> GPIO1.
- `Confirm` -> GPIO5.
- `Up/PageBack` -> GPIO6.
- `Down/PageForward` -> GPIO4.
- `Power/Menu` -> GPIO2 long press or short press, depending UX.

Touch can be integrated later as an additional input source. It should not block the first hardware port.

### 5. Rework Battery And Power Handling

The existing `BatteryMonitor` can be reused if the Murphy battery ADC divider is identified. The vendor factory source does not show battery ADC reads, and the schematic needs a focused pass to map the charger/battery monitor output.

Known power lines that must be controlled:

- GPIO7: display 3.3 V enable.
- GPIO42: SD 3.3 V enable.
- GPIO41: power/status LED.

Deep sleep should initially wake on GPIO2, matching the vendor code. Later, evaluate whether other keys can wake via `ext1`.

## What Must Change In `crosspoint-reader-main`

### 1. Add An ESP32-S3 PlatformIO Environment

Add a new env such as `murphy_m3`:

- Board: start with `esp32-s3-devkitc-1` or a custom board JSON.
- Flash size: `16MB` for this dumped device.
- Flash mode: `dio`.
- PSRAM enabled.
- USB CDC flags for S3 serial as needed.
- Build flag: `BOARD_MURPHY_M3` or `BOARD_CROWPANEL_37_S3`.

The C3 environment should remain intact.

### 2. Partition Strategy

Two sane options exist:

| Strategy | App slots | FS | Tradeoff |
| --- | ---: | ---: | --- |
| Match OEM dump | `0x6d0000` each | `0x200000` | Best compatibility with current firmware layout and OTA slot offsets. |
| Keep CrossPoint layout | `0x640000` each | `0x360000` | More filesystem space, but changes OEM partition offsets. |

The OEM dump layout is:

- `app0` at `0x10000`, size `0x6d0000`.
- `app1` at `0x6e0000`, size `0x6d0000`.
- filesystem at `0xdb0000`, size `0x200000`.
- coredump at `0xff0000`, size `0x10000`.

For first hardware bring-up, matching OEM is lower risk. For long-term CrossPoint, the existing larger filesystem may be useful, but it requires accepting that the OEM flash layout is replaced.

### 3. UI Geometry Pass

CrossPoint is designed around Xteink reader displays:

- X4: 800x480.
- X3: 792x528.
- Several settings/docs assume 480x800 or 800x480.

Murphy is 240x416. Rendering code that uses runtime display dimensions should survive, but layout density, font sizes, page margins, menus, cover thumbnails, and image scaling need testing. Expect a practical UI pass after the display driver works.

### 4. Input Policy

CrossPoint's logical input model assumes seven buttons:

- Back, confirm, left, right, up, down, power.

Murphy has two named buttons and a three-action rotary/key cluster. A workable first policy:

- GPIO1 Back -> `Back`.
- GPIO2 Menu/Home -> `Power` or `Menu`.
- GPIO6 rotary previous -> `Up` and/or `PageBack`.
- GPIO4 rotary next -> `Down` and/or `PageForward`.
- GPIO5 rotary press -> `Confirm`.

Long-press mappings can cover missing logical buttons. Touch gestures can be a later enhancement.

### 5. Storage Bring-Up Is Critical

CrossPoint relies on SD for books and caches. The port is not useful until SD card mount/list/read/write works on the Murphy bus. This should be validated before deeper reader features.

### 6. OTA / Flashing

CrossPoint already uses OTA-style partitions, but the port needs an S3 flashing workflow:

- bootloader at `0x0000`.
- partition table at `0x8000`.
- app at `0x10000`.
- use the chosen partition CSV.
- confirm app size is under the chosen slot limit.

The OEM firmware had `app1` erased and `otadata` selecting `app0`; no useful OEM app rollback contents are present.

## Minimum Viable Port

The first usable Murphy build should target:

- ESP32-S3 PlatformIO build with PSRAM enabled.
- UC8253 black/white full-screen display refresh.
- SD card power enable and mount/list/read.
- Digital button/rotary input.
- Basic CrossPoint navigation and book open/page turn.
- Deep sleep wake from GPIO2.
- Battery percentage hidden or stubbed until ADC path is confirmed.
- Touch ignored.
- Partial refresh/grayscale disabled until the UC8253 path is stable.

## Work Breakdown

1. Add board profile constants and a `murphy_m3` PlatformIO env.
2. Build CrossPoint for ESP32-S3 with display/input temporarily stubbed.
3. Port the vendor UC8253 driver into `EInkDisplay` as a new panel mode.
4. Verify display with clear screen, checkerboard, text, and full refresh.
5. Add configurable SD SPI/power support in `SDCardManager`; verify card listing and file reads.
6. Add a digital input manager for GPIO1/2/4/5/6 and map to CrossPoint logical actions.
7. Wire display/input/storage through `HalGPIO`/`HalDisplay` using board flags instead of X3/X4 detection.
8. Audit hardcoded geometry in CrossPoint and adjust layouts for 240x416.
9. Add sleep/wake and power-enable sequencing.
10. Identify battery ADC/divider from schematic or hardware probing and enable `BatteryMonitor`.
11. Decide partition layout and document flashing/rollback behavior.
12. Run a hardware validation matrix: display, SD, input, WiFi, BLE disabled/enabled, sleep wake, OTA, book rendering.

## Main Risks / Unknowns

| Area | Risk |
| --- | --- |
| Flash size | Vendor docs say 8 MB, dump is 16 MiB. Use dump for this device, but verify physical module markings if shipping images. |
| UC8253 waveform quality | Vendor code is simple and should work for black/white; partial refresh and grayscale quality will require tuning. |
| SD bus | Current SDK assumes one default SPI bus. Murphy uses a separate SD SPI bus and power enable. |
| Input UX | CrossPoint expects more buttons than Murphy exposes directly. The rotary/key mapping needs a real usability pass. |
| Battery | ADC pin/divider not identified yet. |
| GPIO0/strapping | Avoid relying on boot strapping pins for normal input unless verified. |
| Sleep current | Display and SD power enables must be off in sleep or battery life will be poor. |
| App size | Need an S3 build to confirm the app fits whichever OTA slot size is chosen. |
| Touch | Present but intentionally outside the first port; later it can replace or augment missing buttons. |

## Bottom Line

The port is a moderate hardware enablement project, not a rewrite. The app architecture can likely survive, but `community-sdk` needs to grow from an X3/X4 SDK into a board-profile SDK. The highest-leverage first milestone is a Murphy board profile plus three hardware bring-up tests: display refresh, SD card mount, and button/rotary events.
