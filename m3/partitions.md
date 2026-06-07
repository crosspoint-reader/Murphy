# Partition Layout

| Name      | Type | Offset     | Size       | Size (Human) |
| --------- | ---- | ---------- | ---------- | ------------ |
| nvs       | data | 0x009000   | 0x005000   | 20.0 KB      |
| ota       | data | 0x00E000   | 0x002000   | 8.0 KB       |
| ota_0     | app  | 0x010000   | 0x6D0000   | 6.81 MB      |
| ota_1     | app  | 0x6E0000   | 0x6D0000   | 6.81 MB      |
| spiffs    | data | 0xDB0000   | 0x200000   | 2.00 MB      |
| coredump  | data | 0xFF0000   | 0x010000   | 64.0 KB      |

## Notes

- Total flash usage ends at `0xFFFFFF` (16 MB flash).
- Dual OTA app slots (`ota_0` / `ota_1`) enable A/B firmware updates.
- `nvs` stores non-volatile key/value config; `ota` holds the OTA data selector.
- `spiffs` provides a 2 MB filesystem partition.
- `coredump` reserves 64 KB for crash dumps.
