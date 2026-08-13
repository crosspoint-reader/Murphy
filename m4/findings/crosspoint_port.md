# Build, install, and restore CrossPoint

## Status and provenance

The tested port is based on CrossPoint Reader commit `4e619035` on its `develop`
branch and FreeInk SDK commit `a485dc4`. The source changes add a `murphy_m4`
PlatformIO environment and board profile; they should ultimately be reviewed in
their respective upstream repositories.

The clean publication sequence is:

1. Submit the board profile, touch driver, and granular frontlight support to
   FreeInk SDK.
2. Point the CrossPoint submodule at that reviewed SDK revision and submit the
   M4 environment, HAL integration, settings, gestures, and frontlight screen.
3. Link both source pull requests from the Murphy documentation pull request.

This keeps hardware drivers in the SDK and application behavior in CrossPoint,
matching the existing project architecture instead of vendoring a second copy
of either codebase here.

This documentation does not distribute a personal factory flash dump. Create and
verify your own backup before installing the port.

## Prerequisites

- A data-capable USB-C cable
- Python 3.12 or another version supported by current PlatformIO tooling
- PlatformIO/pioarduino
- `esptool`
- A recursive CrossPoint checkout containing the corresponding FreeInk SDK changes

Build from the CrossPoint repository root:

```sh
pio run -e murphy_m4
```

The application image is written to:

```text
.pio/build/murphy_m4/firmware.bin
```

## Back up the factory installation

Enter download mode by holding the bottom side button, pressing and releasing
the recessed reset button, waiting about one second, and then releasing the side
button. Identify the serial device instead of assuming the example path below.

```sh
esptool --chip esp32s3 --port /dev/cu.usbmodem101 \
  read-flash 0 0x1000000 murphy-m4-factory-backup.bin
shasum -a 256 murphy-m4-factory-backup.bin
```

Store the backup and checksum privately in at least two locations. The image may
contain credentials and identifiers.

## Install the application-only build

With the device still in download mode:

```sh
esptool --chip esp32s3 --port /dev/cu.usbmodem101 --baud 921600 \
  write-flash 0x10000 .pio/build/murphy_m4/firmware.bin
```

This deliberately writes only the existing `app0` application partition. It
does not replace the bootloader, partition table, NVS, filesystem, second OTA
slot, or touch-controller firmware.

## Monitor startup

```sh
pio device monitor --port /dev/cu.usbmodem101 --baud 115200 \
  | tee murphy-m4-monitor.log
```

Reset or power-cycle the reader after starting the monitor. Verify display,
touch, buttons, SD access, battery, frontlight, sleep/wake, and Wi-Fi before
considering a build suitable for wider testing.

## Restore the factory image

Enter download mode again, then write the complete backup made from the same
physical device:

```sh
esptool --chip esp32s3 --port /dev/cu.usbmodem101 --baud 921600 \
  write-flash 0x0 murphy-m4-factory-backup.bin
```

Do not restore another person's full dump: it may contain device-specific data,
credentials, calibration, or identifiers.
