# Storage and NVS

## Filesystem Partition

Partition label: `spiffs`  
Offset: `0xdb0000`  
Size: `0x200000`

The partition is 100% `0xff`, so no filesystem files were recoverable from this dump. The app itself references LittleFS and `/littlefs`, so the partition label is probably historical or generic.

## NVS Partition

Partition: `analysis/extracted/nvs.bin`  
Size: 20 KiB  
State: populated

The NVS dump contains readable keys and values. Treat it as sensitive. Recovered examples include:

- Namespaces/keys: `eeprom`, `nvs.net80211`, `cal_data`, `cal_mac`, `ap.ssid`, `ap.passwd`, `sta.ssid`, `sta.pswd`, `sta.scan_method`, `sta.sort_method`, `sta.pmf_e`.
- Access-point SSID/value: `E-Paper Reader`
- Access-point password-like value: `88888888`
- Station SSID/value: `COROGOO`
- Station password-like value: `s580716777s`

Because NVS stores multiple pages and old entries, repeated values may include stale history as well as current state. A proper NVS parser would be the next step before declaring which duplicate entry is active.

## Erased Regions

These partitions are all erased:

- `app1.bin`: 100% `0xff`
- `spiffs.bin`: 100% `0xff`
- `coredump.bin`: 100% `0xff`

