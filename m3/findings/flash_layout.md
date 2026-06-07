# Flash Layout

## Binwalk

`binwalk` 3.1.0 was run first. Its generic signature scan did not recognize ESP-IDF partition-table records, and only reported crypto/constants inside the app image:

| Offset | Finding |
| ---: | --- |
| `0x0b20ec` | SHA256 hash constants |
| `0x0b5800` | CRC32 polynomial table |
| `0x0b87f4` | copyright text |
| `0x0f82fc` | AES S-box |
| `0x0ffed4` | AES S-box |

Because this is an ESP32-S3 flash image, partitions were carved from the ESP-IDF partition table at `0x8000`. The requested `m3/binwalk_extracted/` directory exists and contains `esp-partitions/`.

## ESP-IDF Partition Table

| Name | Type | Subtype | Offset | Size | State |
| --- | ---: | ---: | ---: | ---: | --- |
| `nvs` | `0x01` | `0x02` | `0x009000` | `0x005000` | populated |
| `otadata` | `0x01` | `0x00` | `0x00e000` | `0x002000` | selects app0 |
| `app0` | `0x00` | `0x10` | `0x010000` | `0x6d0000` | valid ESP32-S3 app |
| `app1` | `0x00` | `0x11` | `0x6e0000` | `0x6d0000` | erased, all `0xff` |
| `spiffs` | `0x01` | `0x82` | `0xdb0000` | `0x200000` | erased, all `0xff` |
| `coredump` | `0x01` | `0x03` | `0xff0000` | `0x010000` | erased, all `0xff` |

The partition label says `spiffs`, but the firmware strings use `/littlefs` and Arduino `LittleFS.cpp`. The partition is erased in this dump, so there are no files to recover from it.

## Extracted Partition Hashes

| File | SHA256 |
| --- | --- |
| `m3/extracted/bootloader.bin` | `6ac230d6941c798d91b83334598bb2266a417200aec3b15c74d4d69787e6a269` |
| `m3/extracted/partition-table.bin` | `e26311cd92e39e998672a3f8e4a7d3742d3288f7d6bcbca96a7d78df5d98bb35` |
| `m3/extracted/nvs.bin` | `e208573b0c631f421ecf1c578aabdf69716858d41762dd8b80b5854bd0afbb5e` |
| `m3/extracted/otadata.bin` | `f94c5d786a7a8fab06ac5d10e33bf37711a6697636dc037559ea19cc410a17f0` |
| `m3/extracted/app0.bin` | `aa7daab8636cde10e4d57cb12c8cc272908847870ea3f319343c90026c2ece4d` |
| `m3/extracted/app1.bin` | `07b35687ffead33938b3663d20026d9a2db405a1013a9f7b66007bbcb282e59e` |
| `m3/extracted/spiffs.bin` | `4bda3a28f4ffe603c0ec1258c0034d65a1a0d35ab7bd523a834608adabf03cc5` |
| `m3/extracted/coredump.bin` | `71189f7fb6aed638640078fba3a35fda6c39c8962e74dcc75935aac948da9063` |

