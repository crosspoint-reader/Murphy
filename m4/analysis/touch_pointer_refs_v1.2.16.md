# Pointer Reference Report

## `FT6336`

- string `00007dcf`: `"%s rejected invalid FT6336U status=0x%02X points=%u addr=0x%02X frame: %02X %02X %02X %02X %02X\n"`
  - direct refs: none
- string `00007e37`: `"Mofei FT6336U config failed on SDA=%d SCL=%d addr=0x%02X\n"`
  - direct ref from `00062790` in `<no function>`
  - direct ref from `00069c44` in `<no function>`
  - pointer `00062790` in block `ram`
    - pointer refs: none
  - pointer `00069c44` in block `ram`
    - pointer refs: none
  - pointer `3c252790` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none
  - pointer `3c259c44` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none
- string `00007e9f`: `"FT6336U late-init config failed addr=0x%02X\n"`
  - direct ref from `00062224` in `<no function>`
  - direct ref from `000698a8` in `<no function>`
  - pointer `00062224` in block `ram`
    - pointer refs: none
  - pointer `000698a8` in block `ram`
    - pointer refs: none
  - pointer `000e0d8c` in block `ram`
    - pointer refs: none
  - pointer `3c252224` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none
  - pointer `3c2598a8` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none
  - pointer `3c2d0d8c` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none

## `late-init`

- string `00007e9f`: `"FT6336U late-init config failed addr=0x%02X\n"`
  - direct ref from `00062224` in `<no function>`
  - direct ref from `000698a8` in `<no function>`
  - pointer `00062224` in block `ram`
    - pointer refs: none
  - pointer `000698a8` in block `ram`
    - pointer refs: none
  - pointer `000e0d8c` in block `ram`
    - pointer refs: none
  - pointer `3c252224` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none
  - pointer `3c2598a8` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none
  - pointer `3c2d0d8c` in block `esp32s3_seg0_3c1f0020`
    - pointer refs: none

## `touchTask`
