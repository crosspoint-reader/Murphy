# Murphy M4 — USB Connection & Flash Access

## Summary

The Murphy M4's USB-C port **does carry data** — the data lines are wired to the
ESP32-S3's native USB peripheral (GPIO19/20). It is not charge-only. However, it
does not use a separate USB-to-serial bridge chip (like CH340 or CP2102 on many
dev boards). Instead, the ESP32-S3 implements USB directly in silicon.

---

## USB Modes

The Murphy M4 firmware can expose the USB port in two different modes:

| Mode | What appears on Mac | How to enter |
|------|---------------------|--------------|
| **Disk mode** | `MOFEI Storage` — the SD card as a USB drive | Via Settings in the reader UI |
| **Normal/serial mode** | `/dev/cu.usbmodem101` — a CDC serial port | Just run the firmware normally |
| **Download mode (ROM)** | `/dev/cu.usbmodem101` — ROM bootloader | Hold GPIO0 (bottom side button) during reset |

Disk mode was identified via `system_profiler SPUSBDataType`:
```
Product ID: 0x1001
Vendor ID:  0x303a  (Espressif Systems)
Manufacturer: ESP32
```

The Mac's JumpCloud MDM policy blocked mounting the SD card as a disk, but
the USB enumeration itself was never blocked. Serial/JTAG mode is unaffected
by this policy.

---

## Download Mode

**Download mode** is a special ROM bootloader built into every ESP32 chip. It is
stored in the chip's internal read-only memory (not in external flash), so it
always works even if the external flash is corrupted or erased. When the chip
enters download mode, it waits for `esptool` commands over USB to read or write flash.

### Entry sequence (Murphy M4)

```
1. Device powered on and running normally
2. Hold: bottom side button (this is GPIO0, the strapping pin)
3. Press and release: recessed RESET button (below the bottom side button)
4. Keep holding: bottom side button for ~1 more second
5. Release: bottom side button
```

The device screen goes blank/freezes. A new port appears in `/dev/cu.*` and
**stays there** (unlike normal boot, where it flickers and disappears).

### Why GPIO0 matters

GPIO0 is ESP32's "strapping pin". The ROM reads its state during the first
milliseconds of boot:
- GPIO0 = HIGH (floating or pulled up) → normal boot → loads firmware from flash
- GPIO0 = LOW (held down) → download mode → waits for esptool

The bottom side button pulls GPIO0 LOW when pressed. By holding it during reset,
we force the chip into download mode before it has a chance to start the firmware.

### Detect the serial port

```bash
ls /dev/cu.*
```

---

## Flash Dump

A full 16 MB flash backup was taken on 2026-08-04:

```
esptool --port /dev/cu.usbmodem101 --chip esp32s3 \
    read-flash 0 0x1000000 /path/to/murphy-m4-factory-backup.bin
```

Store the dump outside the repository. It may contain credentials, device
identifiers, reading state, and other private data.

This is the safety net for all future experiments. If anything goes wrong,
restore with:

```
esptool --port /dev/cu.usbmodem101 --chip esp32s3 \
    write-flash 0x0 /path/to/murphy-m4-factory-backup.bin
```

---

## esptool

`esptool` is Espressif's official command-line tool for communicating with ESP32
chips in download mode. It can:
- Read chip info (`flash-id`)
- Read flash memory to a file (`read-flash`)
- Write a binary to flash (`write-flash`)
- Erase flash (`erase-flash`)

Key flags used:
- `--chip esp32s3` — tells esptool which ROM protocol to use
- `--no-stub` — skip uploading a faster stub flasher (sometimes needed for timing)
- `--before no_reset` — don't try to auto-reset (needed when already in download mode via USB)

### Flash address layout

ESP32 flash is split into named regions. Each must be written at the correct
address or the chip won't boot:

| Region | Address | Contents |
|--------|---------|----------|
| Second-stage bootloader | `0x0000` | Initialises RAM, loads the app |
| Partition table | `0x8000` | Tells the chip where each partition lives |
| App partition (ota_0) | `0x10000` | The actual firmware |

**This is what went wrong in our first probe attempt.** We flashed the app
binary (`firmware.bin`) at address `0x0`, overwriting the bootloader.
The chip's ROM then found a hash mismatch and entered a reboot loop.
The fix: always flash bootloader + partitions + app at their respective addresses
using `flash_probe.sh`.

---

## Chip Details (from esptool)

```
Chip:        ESP32-S3 (QFN56) revision v0.2
Features:    Wi-Fi, BT 5 (LE), Dual Core + LP Core, 240MHz
PSRAM:       8 MB embedded (AP_3v3)
Flash:       16 MB, quad mode, 3.3V
Crystal:     40 MHz
USB mode:    USB-Serial/JTAG (native, GPIO19/20)
```

---

## Partition Table

Extracted from the flash dump at offset `0x8000`:

| Name | Type | Offset | Size | Notes |
|------|------|--------|------|-------|
| nvs | data/nvs | 0x9000 | 20 KB | WiFi credentials, settings |
| otadata | data/otadata | 0xe000 | 8 KB | Tracks which OTA slot is active |
| app0 | app/ota_0 | 0x10000 | 6.81 MB | Active application slot |
| app1 | app/ota_1 | 0x6e0000 | 6.81 MB | OTA update slot (currently erased) |
| spiffs | data/fat | 0xdb0000 | 2 MB | Internal filesystem |
| coredump | data/coredump | 0xff0000 | 64 KB | Crash dumps |

The two app slots enable safe OTA updates: the new firmware goes into the empty
slot, and only becomes permanent after it calls `esp_ota_mark_app_valid()`. If
it crashes before that, the chip rolls back to the old slot automatically.
