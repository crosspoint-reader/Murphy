#!/usr/bin/env bash
# Flash a probe sketch to Murphy M4.
# PlatformIO builds three separate binaries; they must go to three different addresses.
# Flashing only firmware.bin at 0x0 overwrites the bootloader — don't do that.
#
# Usage: PORT=/dev/cu.usbmodem101 ./flash_probe.sh i2c_scan
#
# The device must be in download mode:
#   Hold bottom side button (GPIO0) + press recessed RESET button → release both

set -e

ENV=${1:-i2c_scan}
: "${PORT:?Set PORT to the device serial port, for example /dev/cu.usbmodem101}"
BUILD=".pio/build/$ENV"

if [ ! -f "$BUILD/firmware.bin" ]; then
    echo "Error: $BUILD/firmware.bin not found. Run: pio run -e $ENV"
    exit 1
fi

echo "=== Flashing probe '$ENV' to Murphy M4 ==="
echo "Port:      $PORT"
echo "Env:       $ENV"
echo ""
echo "Enter download mode now, then wait 3 seconds:"
echo "  Hold bottom side button (GPIO0) + press recessed RESET → release both"
echo ""
sleep 3

python3 -m esptool --port "$PORT" --chip esp32s3 --baud 115200 write-flash \
    0x0     "$BUILD/bootloader.bin" \
    0x8000  "$BUILD/partitions.bin" \
    0x10000 "$BUILD/firmware.bin"

echo ""
echo "Done. Device will reboot into the probe."
echo "Open serial monitor: screen $PORT 115200"
echo ""
echo "Restore the private full-flash backup for this device after testing."
