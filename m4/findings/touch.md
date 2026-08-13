# Touch controller

## Result

Touch input works using the same high-level sequence recovered from the factory
MurphyOS v1.2.16 application: power the controller, reset it, configure three
volatile runtime registers, then read touch frames from register `0x02` when the
interrupt line is asserted.

The factory binary calls this controller `FT6336U`, but its working address and
wiring differ from common FT6336U modules. Until the package marking or schematic
is independently checked, this repository describes it conservatively as the
**factory-labelled FT6336U-compatible controller**.

| Signal or property | Confirmed value |
|---|---|
| SDA | GPIO13 |
| SCL | GPIO12 |
| 7-bit I²C address | `0x2E` |
| Interrupt | GPIO44, active low |
| Power enable | GPIO45, active low |
| Reset | GPIO7, shared with display reset |
| I²C controller | ESP-IDF master bus 1, 100 kHz |
| Coordinate transform | swap X/Y, then flip Y |

Earlier probes appeared to find a device at `0x38` on GPIO10/GPIO4. That result
did not produce usable coordinate frames and is not the working touch path.
GPIO10 is the SD-card power enable and GPIO4 is the display clock.

## Startup sequence

The implemented startup sequence is:

1. Drive GPIO45 low and wait 500 ms for the touch rail to settle.
2. Pulse GPIO7 low for 50 ms, then high and wait 100 ms.
3. Configure GPIO44 as an input with pull-up.
4. Open I²C controller 1 on GPIO13/GPIO12 at 100 kHz, address `0x2E`.
5. Write the following ordinary runtime registers:

   | Register | Value | Role inferred from factory code / FT6x36 convention |
   |---|---:|---|
   | `0x00` | `0x00` | operating mode |
   | `0x80` | `0x16` | touch threshold |
   | `0x88` | `0x04` | report-rate configuration |

6. Read 11 bytes starting at register `0x02` while interrupt is low or a touch
   remains active.

Frames are validated before generating events because invalid repeated-byte
patterns were observed during investigation. Coordinates are transformed for
the portrait-mounted sensor and landscape framebuffer.

## Firmware safety boundary

These register writes are volatile initialization, comparable to setting a
sensitivity or report rate after power-up. The driver contains no controller
firmware blob and no erase, bootloader, flash-write, or upgrade operation.

Future contributions must preserve this boundary. Any proposed touch-controller
firmware update belongs in a separate, opt-in research tool with explicit risk
documentation; it must not be part of the reader firmware or normal installation.
