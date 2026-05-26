# Firmware Identity

## Bootloader

`analysis/extracted/bootloader.bin` is a valid ESP32-S3 image:

- Entry point: `0x403c98d0`
- Segments: 3
- Flash size: 16 MiB
- Flash frequency: 80 MHz
- Flash mode: DIO
- Chip ID: 9, ESP32-S3
- Validation hash: valid

Embedded bootloader strings include ESP-IDF paths such as:

- `/IDF/components/bootloader_support/src/bootloader_flash.c`
- `/IDF/components/bootloader_support/src/esp32s3/bootloader_sha.c`
- `/IDF/components/esp_hw_support/port/esp32s3/rtc_clk.c`

## App Image

`analysis/extracted/app0.bin` is the active valid app image:

- Image type: ESP32-S3
- Entry point: `0x40377b70`
- Segments: 7
- Flash size: 16 MiB
- Flash frequency: 80 MHz
- Flash mode: DIO
- Validation hash: `d4e3bd0c34f93becbbba37e0ceba4173c636dd546f7aec6de61b85f5b9b45459`
- Project name: `arduino-lib-builder`
- App version: `esp-idf: v4.4.7 38eeba213a`
- Compile time: `Mar 5 2024 12:12:53`
- ELF SHA256: `a6f205d9e091d605cf2d0e1684ee43f08ae63ff52edcdd3becbf81d62563ca3b`
- ESP-IDF: `v4.4.7-dirty`
- Secure version: `0`

`analysis/extracted/app1.bin` is not a valid image and is all `0xff`.

## Toolchain and SDK Evidence

The firmware includes many source paths from Arduino-ESP32 and PlatformIO:

- `C:/Users/HZW/.platformio/packages/framework-arduinoespressif32/libraries/SPI/src/SPI.cpp`
- `C:/Users/HZW/.platformio/packages/framework-arduinoespressif32/libraries/BLE/src/BLEDevice.cpp`
- `C:/Users/HZW/.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src/WiFiSTA.cpp`
- `C:/Users/HZW/.platformio/packages/framework-arduinoespressif32/libraries/LittleFS/src/LittleFS.cpp`
- `.pio/libdeps/esp32-s3-devkitc-1/ESP32-audioI2S-3.0.12/src/Audio.cpp`

Conclusion: this was almost certainly built as an Arduino-ESP32/PlatformIO project targeting an ESP32-S3 devkit-style environment, with ESP-IDF 4.4.7 underneath.

