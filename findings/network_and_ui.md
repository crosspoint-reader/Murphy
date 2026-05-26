# Network, Web UI, and OTA

## Local Web UI

The app embeds HTML/CSS/JavaScript for a local configuration UI. Recovered endpoints include:

| Endpoint | Purpose inferred from surrounding strings |
| --- | --- |
| `/Read_scanWifi` | Return scanned WiFi list |
| `/Wifi` | Save/connect WiFi config |
| `/Weather` | Save weather config |
| `/Read_UID` | Read device UID |
| `/Read_LANGUAGE` | Read UI language |
| `/Read_WeatherKey` | Read weather API key |
| `/Read_WeatherCity` | Read weather city ID |
| `/Read_WeatherHost` | Read weather API host |
| `/Read_Ver` | Read firmware version |
| `/update` | HTTP firmware upload/update |

The UI has tabs for WiFi and firmware update, upload progress, language strings in Chinese and English, and weather configuration fields.

## OTA / Firmware Update

Recovered update paths and strings:

- `/firmware.bin`
- `/firmware_OTA.bin`
- `/update`
- `displayOtaTask`
- `Check for updates`
- `Already the latest version`
- `Version too low, please use Wired firmware flashing!`
- `Download update file`
- `Could Not Activate The Firmware`

Remote update metadata URL:

`https://gitee.com/corogoo/3.7-inch-ink-screen-reader/raw/master/firmware/touch/update.json`

The partition table has dual OTA slots, but only `app0` is populated in this dump. `otadata` starts with `01 00 00 00`, consistent with selecting OTA slot 0.

## WiFi / BLE

WiFi and BLE are both linked:

- Arduino WiFi sources: `WiFiAP.cpp`, `WiFiGeneric.cpp`, `WiFiSTA.cpp`, `WiFiUdp.cpp`.
- BLE sources: `BLEDevice.cpp`, `BLEScan.cpp`, `BLEClient.cpp`, `BLERemoteCharacteristic.cpp`.
- Application strings: `bleScan`, `scanAndConnect %s`, `Bluetooth disabled`, `Scanning for devices`.

The device likely starts a setup hotspot. UI strings show:

- Hotspot name: `E-Paper Reader`
- Hotspot password: `88888888`

