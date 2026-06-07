#!/usr/bin/env bash
PORT="/dev/cu.usbmodem83201"
CHIP="esp32s3"
BAUD="921600"
BIN="/Users/jmitch/GitHub/Murphy_M3/m3_flash_dump.bin"

echo "Hammering flash on $PORT until it sticks..."
echo "Press Ctrl+C to abort."
echo ""

ATTEMPT=0
while true; do
    ATTEMPT=$((ATTEMPT + 1))
    esptool --port "$PORT" --chip "$CHIP" --baud 115200 --no-stub --before default_reset write-flash 0x0 "$BIN" 2>/dev/null && {
        echo ""
        echo ">>> Flash succeeded on attempt $ATTEMPT."
        exit 0
    }
    sleep 0.1
done
