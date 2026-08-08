# Technical Concepts & Tools Reference

A plain-language explanation of every technology involved in this porting project.

---

## The Hardware

### ESP32-S3

A microcontroller chip made by Espressif Systems. It is the "brain" of both the
Murphy M3 and M4 e-readers. Key facts:

- **Dual-core Xtensa LX7 CPU** at up to 240 MHz — fast enough to render EPUB
  pages, handle WiFi, and drive an e-ink display simultaneously
- **520 KB internal SRAM** — the chip's working memory (stack, heap, variables)
- **8 MB PSRAM** (on the M4) — extra RAM on a separate chip, accessed via SPI.
  Needed for the large e-ink framebuffer and font caches
- **16 MB external Flash** — like a tiny SSD, stores the firmware code and any
  read-only data (fonts, images)
- **GPIO pins** (General Purpose Input/Output) — physical connections on the chip
  that can be individually configured as inputs (read a button) or outputs
  (drive a signal). The ESP32-S3 has 48 of them, numbered GPIO0–GPIO48
- **Native USB** — USB-Serial/JTAG is built directly into the chip on GPIO19/20,
  no separate USB bridge chip needed
- **Peripherals** — built-in hardware blocks for common protocols: SPI, I2C,
  SDMMC, LEDC (PWM), UART, etc. Much faster and more efficient than doing
  the same thing in software

### GPIO Strapping Pins

A few GPIO pins are "strapping pins" — the chip reads them during the very first
milliseconds of boot, before any firmware runs, to decide what mode to enter.
GPIO0 is the key one: LOW = download mode, HIGH/floating = normal boot.

---

## Protocols

### SPI (Serial Peripheral Interface)

A 4-wire synchronous serial protocol for fast communication between chips.
Used for the e-ink display and (in M3) the SD card. Wires:
- **SCLK** (clock) — master sends a pulse for each bit
- **MOSI** (Master Out Slave In) — data from CPU to display
- **MISO** (Master In Slave Out) — data from display to CPU (often unused for e-ink)
- **CS** (Chip Select) — pulled LOW to address one specific device

For e-ink displays, SPI is typically bit-banged (software toggles GPIO pins
manually) rather than using the hardware SPI peripheral, because the display
protocol needs precise timing control that hardware SPI doesn't provide.

### I2C (Inter-Integrated Circuit)

A 2-wire protocol for slower, multi-device communication. Used for the
touch controller and audio codec. Wires:
- **SDA** (Serial Data)
- **SCL** (Serial Clock)

Each device on an I2C bus has a 7-bit address (the M4 touch controller uses `0x2E`).
Multiple devices share the same two wires — the master addresses them
individually by sending their address first.

### SD_MMC / SDIO (4-bit mode)

The SD card standard has two modes: SPI (slow, 1 data wire) and SDIO (fast,
4 data wires). The M4 uses 4-bit SDIO, which is 4× faster and uses a
dedicated hardware peripheral in the ESP32-S3. Wires:
- **CLK** — clock
- **CMD** — command/response
- **D0–D3** — 4 parallel data lines

### LEDC / PWM (Pulse Width Modulation)

The ESP32-S3 has a "LEDC" peripheral (LED Controller) that generates PWM signals
on any GPIO. PWM rapidly switches a pin between HIGH and LOW at a fixed frequency.
By varying the duty cycle (fraction of time spent HIGH), you control apparent
brightness. The M4's frontlight LEDs are dimmed this way.

---

## The Software Stack

### Arduino Framework

A programming model that simplifies embedded development. Every Arduino program
has two functions:
- `setup()` — runs once at boot, initialise hardware
- `loop()` — runs forever, handle events

CrossPoint is written as an Arduino sketch but uses C++ classes extensively.
The "framework" provides the `Serial`, `Wire`, `SPI`, `SD_MMC` APIs, among others.

### ESP-IDF (under Arduino)

Arduino-ESP32 is built on top of Espressif's own SDK, ESP-IDF. CrossPoint
sometimes dips into ESP-IDF directly for things Arduino doesn't expose cleanly
(e.g. LEDC, SDMMC, deep sleep, OTA). When you see includes like
`#include <driver/ledc.h>`, that's ESP-IDF.

### PlatformIO

A build system and package manager for embedded firmware. It replaces the Arduino
IDE for serious projects. Key concepts:

- **`platformio.ini`** — project configuration. Defines "environments", each of
  which targets a specific board, sets build flags, and lists dependencies
- **`pio run -e <env>`** — compile the firmware for that environment
- **`pio run -e <env> -t upload`** — compile and flash to device
- **`.pio/build/<env>/`** — output directory containing the compiled binaries

PlatformIO automatically downloads the correct compiler toolchain, Arduino
framework version, and any libraries listed in `lib_deps`.

### freeink-sdk

An open-source hardware abstraction library specifically for e-ink readers,
maintained by the CrossPoint project. CrossPoint depends on it as a git submodule.

Key components:
- **`BoardConfig`** — a database of hardware profiles. Each supported device
  (Xteink X4, Murphy M3, de-link, etc.) has a `constexpr BoardProfile` struct
  listing every pin, display size, and capability. CrossPoint reads
  `BoardConfig::ACTIVE` at runtime to know its own hardware.
- **`EInkDisplay`** — display driver. Supports multiple controllers (UC8253,
  SSD1677, etc.) and handles the complex waveform sequences.
- **`InputManager`** — button and touch event queue. Normalises physical button
  presses and touch coordinates into logical events.
- **`SDCardManager`** — SD card I/O with mutex protection for thread safety.

### HAL (Hardware Abstraction Layer)

CrossPoint's own thin wrapper over the freeink-sdk. Lives in `lib/hal/`.
The rest of the CrossPoint codebase only calls HAL functions, never the SDK
or hardware directly. This means adding a new device only requires changes
in the HAL and SDK — the UI and reader logic are untouched.

```
CrossPoint UI  →  HAL  →  freeink-sdk  →  hardware
```

### esptool

Espressif's official command-line tool for communicating with ESP32 chips in
download mode. Python-based, installed via pip. Used to:
- Identify the chip (`flash-id`)
- Read all 16 MB of flash to a file (`read-flash`)
- Write firmware to flash (`write-flash`)

**Critical detail:** ESP32 flash has three separate regions that must be written
at specific addresses. Writing the wrong binary to the wrong address corrupts
the bootloader and causes a boot loop:

| Binary | Address | What it is |
|--------|---------|------------|
| `bootloader.bin` | `0x0000` | Second-stage bootloader (loads the app) |
| `partitions.bin` | `0x8000` | Partition table |
| `firmware.bin` | `0x10000` | The actual application |

---

## E-ink Display Concepts

### How E-Ink Works

E-ink panels contain millions of tiny microcapsules, each filled with black
and white charged particles suspended in clear oil. Applying a voltage field
pulls the particles to one surface or the other, making each pixel black or white.
The image persists after power is removed (bistable display) — which is why
e-readers have such excellent battery life.

### Framebuffer

The display's pixel data is stored in RAM as a 1-bit-per-pixel buffer:
1 = white, 0 = black. For 480×800 pixels: 480 × 800 ÷ 8 = 48,000 bytes.
CrossPoint writes to this buffer, then calls `displayBuffer()` to push it
to the panel. The push involves sending all 48,000 bytes over SPI and then
triggering a refresh cycle.

### Waveforms / LUTs

Changing a pixel from white to black (or vice versa) isn't instant. The controller
runs a sequence of voltage pulses defined by lookup tables (LUTs). Different
waveforms trade off between speed, ghosting, and image quality:
- **Full refresh** — high voltage, many cycles, ~2 seconds, no ghosting
- **Partial refresh** — fewer cycles, ~300ms, may leave faint ghosts
- **Fast refresh** — minimal cycles, ~100ms, visible ghosting

The LUTs are specific to each display controller and panel combination.
This is why identifying the M4's display controller is critical — using the
wrong LUTs produces a garbled or blank image.

---

## OTA (Over-The-Air) Updates

OTA means delivering a new firmware binary over WiFi instead of USB. The ESP32
has dedicated OTA support in ESP-IDF:

1. The partition table reserves two app slots (`app0` and `app1`)
2. The running firmware downloads the new binary and writes it to the empty slot
3. `otadata` is updated to mark the new slot as pending
4. On reboot, the ROM checks `otadata` and boots the new slot
5. If the new firmware calls `esp_ota_mark_app_valid()`, it becomes permanent
6. If it crashes before doing so, the ROM rolls back to the previous slot

The Murphy M4 OTA endpoint is `http://murphy.pandacat.ai/ota/latest`. This
returns a JSON manifest with a `firmware_url` pointing to the actual binary.
We can intercept this with a local DNS override and HTTP server to push
CrossPoint instead of an official MurphyOS update.

---

## Probing Methodology

Since we can't read the M4 schematics, we discover pin assignments experimentally.
The general approach:

1. Write a small Arduino sketch ("probe") that tries a set of candidate pins
2. Build it with PlatformIO (`pio run -e <env>`)
3. Flash via `flash_probe.sh` (handles all three binary addresses correctly)
4. Monitor serial output via `python3 monitor.py <label>` (creates timestamped log)
5. Note which pin combination produces the expected result
6. Restore original firmware from flash dump, repeat for next subsystem

### SD Card Probing — A Deep Dive

The SD card investigation is worth explaining in detail because it hit several
non-obvious obstacles that are common in hardware reverse engineering.

#### The Switched SD_VCC Rail

SD cards don't run on the main 3.3V supply. The board has a separate, switchable
power rail called SD_VCC. A transistor gate (typically a PMOS FET) sits between
the main supply and SD_VCC, controlled by a GPIO. When the card isn't in use,
the firmware cuts SD_VCC to save power and protect the card.

The SD data lines (CMD, D0–D3) have pull-up resistors to SD_VCC. When SD_VCC is
off, the pull-ups have nothing to pull to, so all five lines float LOW. This is
why a GPIO state probe alone can't identify SD pins: with power off, every SD
line looks idle.

CLK has no pull-up — it's driven directly by the SDMMC peripheral.

#### The Power-Detect Probe

Before trying to mount the SD card, we need to know which GPIO controls SD_VCC.
The `sd_power_detect` probe works like this:

1. Read baseline GPIO states (all inputs, no pull-up/down)
2. For each HIGH pin: drive it LOW (testing for PMOS active-low gate)
3. Re-read all GPIOs; look for LOW→HIGH transitions
4. Five pins rising at once = we found the power gate (SD_VCC turned on,
   pulling CMD+D0–D3 HIGH through their pull-up resistors)
5. For each LOW pin: drive it HIGH (testing for NMOS active-high gate)
6. Same observation — five rising pins = power gate found

#### The Back-Driving Artifact

The power-detect probe produced a misleading result: both GPIO15 and GPIO16
appeared to be the power enable, each causing five pins to rise. This is an
artifact of the pull-up network.

When an SD data pin is driven HIGH by the ESP32's output driver (~20 mA),
current flows backward through its pull-up resistor into the SD_VCC node.
That current charges SD_VCC to a small fraction of 3.3V. Other data pins,
whose pull-ups also connect to that same SD_VCC node, see this voltage and
read as HIGH. The effect looks exactly like "SD_VCC turned on" — but SD_VCC
is not actually at full voltage, and the SD card cannot actually communicate.

The tell: when a real power gate is found, the five rising pins stay HIGH
indefinitely. With back-driving, the voltage sags as soon as you stop
driving the output — but our probe didn't check stability.

The lesson: in a switched-rail pull-up network, driving any node HIGH will
back-drive the others. You need a different test to distinguish a real power
gate from a data pin.

#### GPIO10: The Double-Duty Pin

The correct SD power enable is GPIO10. An early, non-working touch probe also
misidentified this pin as I2C SDA. This was
the last pin we suspected because we'd already confirmed it as I2C SDA and
excluded it from the power-detect candidate list.

The factory firmware reconfigures GPIO10 at runtime:
- During I2C communication with touch controller / audio codec: open-drain input
- Before SD card access: output driven LOW (PMOS on → SD_VCC on)
- After SD access: either returned to I2C mode or left driving LOW

The ESP32-S3 GPIO matrix allows any peripheral function to be routed to any
pin, and any pin to be switched between GPIO input, GPIO output, and peripheral
mode at runtime. Pin-sharing like this is legal and occasionally used to reduce
PCB trace routing complexity.

#### Binary Analysis as the Tiebreaker

After the power-detect probe produced misleading results and multiple SD_MMC
probe iterations failed (because the power pin was wrong and because the correct
CMD pin was excluded from the search), we turned to the factory firmware binary.

A private full-flash backup contains the complete factory installation. The
active firmware sits at offset `0x10000` (the `app0` partition). Full backups
must remain private because they can contain credentials and identifiers.

Key technique: **Xtensa LX7 disassembly**. The ESP32-S3 runs the Xtensa LX7
instruction set. The `call8` instruction (function call) loads arguments into
registers `a10`–`a15` before the call:

```
a10 = this (object pointer)
a11 = first argument
a12 = second argument
...
a15 = sixth argument
stack[0] = seventh argument (if needed)
```

Finding the `SDMMCFS::setPins` call in the binary and reading the `movi.n`
(move immediate) instructions before it directly reveals the GPIO pin numbers
passed as arguments. No source code or symbols required — just the compiled binary.

The power enable was found by searching for `gpio_set_level` calls in functions
near the SD init code. The sequence `gpio_set_level(10, 0)` followed by a
5 ms delay followed by SD_MMC re-init is the unmistakable power-cycle-then-reinit
pattern used in every SD driver.

### Probe Inventory

| Probe | Purpose | Key finding |
|-------|---------|-------------|
| `gpio_state_probe.cpp` | Read baseline state of all accessible GPIOs | Cluster {11,14,15,16,17,18} all LOW; identified unexplained HIGH/LOW pins |
| `i2c_scan.cpp` | Historical broad I2C sweep | Produced a GPIO10/GPIO4 result later disproved as the usable touch path |
| `frontlight_probe.cpp` | Pulse each GPIO with PWM, watch LEDs | cool=GPIO47, warm=GPIO48 |
| `sd_power_detect.cpp` | Find SD_VCC power enable by watching pin transitions | Identified SD cluster; back-driving artifact confused power pin |
| `sd_mmc_probe.cpp` (v1–v6) | Brute-force CLK/CMD/D0 combinations | CLK=16 working by v5; CMD=15 missed because GPIO15 reserved as (wrong) power pin |
| `sd_mount_confirm.cpp` | Targeted mount with binary-derived pins | **CONFIRMED**: CLK=16 CMD=15 D0=17 D1=18 D2=11 D3=14 PWR=GPIO10 LOW |

---

## pyenv

A tool for managing multiple Python versions side by side without affecting the
system Python. Used here because PlatformIO's pioarduino platform requires
Python 3.10+, but the user's system has Python 3.9 (needed for work tools).

```bash
pyenv install 3.12.0
pyenv local 3.12.0   # creates .python-version in the project directory
                      # only affects that directory, system Python unchanged
```
