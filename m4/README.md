# Murphy M4

Hardware research and an experimental CrossPoint port for the Murphy M4 e-reader.
The port has been exercised on one retail device and currently supports the
display, touch screen, three side buttons, SD card, battery indicator, Wi-Fi,
sleep/wake, and dual-channel frontlight.

> [!CAUTION]
> This is community work, not an official Murphy or CrossPoint release. Back up
> the complete 16 MiB flash before installing anything. A full backup can contain
> Wi-Fi credentials and other private device data; never commit or share it.

## Port status

| Subsystem | Status | Notes |
|---|---|---|
| ESP32-S3 / PSRAM | Working | ESP32-S3 revision 0.2, 8 MiB PSRAM, 16 MiB flash |
| Display | Working | 4.26-inch 480×800 GDEQ0426T82, SSD1677 |
| Touch | Working | Factory-labelled FT6336U-compatible controller; see [touch findings](findings/touch.md) |
| Buttons | Working | GPIO1, GPIO2, and shared boot/power button GPIO0 |
| SD card | Working | 4-bit SDMMC |
| Battery | Working | GPIO9 ADC through a measured 2:1 divider |
| Frontlight | Working | Independent cool/warm PWM channels and global controls |
| Wi-Fi and reader features | Working | Inherited from CrossPoint; longer-term testing is still needed |
| Audio | Not investigated | Out of scope for the initial reader port |

## Start here

- [Build, install, and restore](findings/crosspoint_port.md)
- [Hardware reference](findings/hardware.md)
- [Touch-controller findings and safety boundary](findings/touch.md)
- [Frontlight controls](findings/frontlight.md)
- [USB and flash access](findings/flash_access.md)
- [Development history and remaining work](findings/porting_crosspoint.md)
- [Technical concepts](findings/concepts.md)

## Safety boundary

The port may reset the touch controller and write ordinary volatile runtime
configuration registers during startup. It does **not** contain a touch firmware
image, enter the controller bootloader, erase controller storage, or invoke a
firmware-update command. Touch-controller firmware modification is explicitly
out of scope.

Only the ESP32 application partition at `0x10000` was written during normal port
testing. The complete factory flash backup remains the recovery source of truth.

## Repository contents

- `findings/` contains reviewed, human-readable results and procedures.
- `analysis/` contains selected reverse-engineering exports supporting those results.
- `probes/` contains hardware-discovery sketches. Build output is intentionally ignored.
- `binwalk_extracted/` contains selected assets extracted from the publicly distributed
  MurphyOS OTA application image.

Personal flash dumps, extracted private partitions, serial logs, Ghidra project
databases, and local tool environments are intentionally excluded from version control.
