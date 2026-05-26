# Hardware Callsite Contexts

## `displayOtaTask`

- string `000046a1`: `"displayOtaTask"`
- string `3c1946a1`: `"displayOtaTask"`

Refs reported: 0

## `Not full refresh`

- string `00003a85`: `"Not full refresh"`
- string `3c193a85`: `"Not full refresh"`
- ref to `Not full refresh` target `3c193a85` from `00117f88` in `<no function>`
  nearby:
  ```asm
  0010bd4d  ill          
  00110011  ill          
  00110014  retw         
  00117f26  s32i.n       a1, a12, 0xc
  00117f28  add.n        a3, a9, a3
  00117f2a  s32i.n       a1, a12, 0xc
  00117fff  movi.n       a0, 0x3a
  0011800a  s32i.n       a1, a12, 0xc
  0011800c  addi.n       a11, a6, 0x1
  0011800e  s32i.n       a1, a12, 0xc
  ```
- ref to `Not full refresh` target `3c193a85` from `3fc9abb8` in `<no function>`
  nearby:
  ```asm
  002bca5d  wsr          a3, INTENABLE
  002bca60  rsync        
  002bca63  mov.n        a2, a4
  002bca65  retw.n       
  002be6f8  entry        a1, 0x20
  002be6fb  retw.n       
  40374000  s32e         a0, a5, -0x2000
  40374003  s32e         a1, a5, -0x4000
  40374006  s32e         a2, a5, -0x8000
  40374009  s32e         a3, a5, 0x0
  ```

Refs reported: 2

## `LUT_ADD`

- string `000048ba`: `"LUT_ADD"`
- string `000048c2`: `"LUT_ADD OK"`
- string `3c1948ba`: `"LUT_ADD"`
- ref to `LUT_ADD` target `3c1948ba` from `00120c44` in `<no function>`
  nearby:
  ```asm
  00118802  l32i.n       a7, a8, 0x1c
  00118804  bltz         a5, 0x00118172
  00118807  call8        0x000af170
  0011880a  l32i.n       a8, a8, 0x1c
  0011880c  l32i.n       a7, a8, 0x1c
  0011880e  bany         0x001187a7, a7, a8
  00122b24  entry        a1, 0x20
  00122b27  l32r         a8, 0x00060024
  00122b2a  movi.n       a9, 0x0
  00122b2c  s8i          a9, a8, 0x0
  ```
- ref to `LUT_ADD` target `3c1948ba` from `42000c44` in `<no function>`
  nearby:
  ```asm
  420002f0  beqz.n       a15, 0x42000325
  420002f2  s32i.n       a1, a12, 0xc
  420002f4  s32i.n       a9, a9, 0x24
  420002f6  s32i.n       a9, a9, 0x4
  420002f8  s32i.n       a9, a9, 0x24
  420002fa  s32i.n       a9, a9, 0x24
  42002b24  entry        a1, 0x20
  42002b27  l32r         a8, 0x41f40024
  42002b2a  movi.n       a9, 0x0
  42002b2c  s8i          a9, a8, 0x0
  ```
- string `3c1948c2`: `"LUT_ADD OK"`
- ref to `LUT_ADD` target `3c1948c2` from `00120c48` in `<no function>`
  nearby:
  ```asm
  00118802  l32i.n       a7, a8, 0x1c
  00118804  bltz         a5, 0x00118172
  00118807  call8        0x000af170
  0011880a  l32i.n       a8, a8, 0x1c
  0011880c  l32i.n       a7, a8, 0x1c
  0011880e  bany         0x001187a7, a7, a8
  00122b24  entry        a1, 0x20
  00122b27  l32r         a8, 0x00060024
  00122b2a  movi.n       a9, 0x0
  00122b2c  s8i          a9, a8, 0x0
  ```
- ref to `LUT_ADD` target `3c1948c2` from `42000c48` in `<no function>`
  nearby:
  ```asm
  420002f0  beqz.n       a15, 0x42000325
  420002f2  s32i.n       a1, a12, 0xc
  420002f4  s32i.n       a9, a9, 0x24
  420002f6  s32i.n       a9, a9, 0x4
  420002f8  s32i.n       a9, a9, 0x24
  420002fa  s32i.n       a9, a9, 0x24
  42002b24  entry        a1, 0x20
  42002b27  l32r         a8, 0x41f40024
  42002b2a  movi.n       a9, 0x0
  42002b2c  s8i          a9, a8, 0x0
  ```

Refs reported: 4

## `LUT_FULL_ADD`

- string `000048da`: `"LUT_FULL_ADD"`
- string `000048e7`: `"LUT_FULL_ADD OK"`
- string `3c1948da`: `"LUT_FULL_ADD"`
- ref to `LUT_FULL_ADD` target `3c1948da` from `00120c50` in `<no function>`
  nearby:
  ```asm
  00118802  l32i.n       a7, a8, 0x1c
  00118804  bltz         a5, 0x00118172
  00118807  call8        0x000af170
  0011880a  l32i.n       a8, a8, 0x1c
  0011880c  l32i.n       a7, a8, 0x1c
  0011880e  bany         0x001187a7, a7, a8
  00122b24  entry        a1, 0x20
  00122b27  l32r         a8, 0x00060024
  00122b2a  movi.n       a9, 0x0
  00122b2c  s8i          a9, a8, 0x0
  ```
- ref to `LUT_FULL_ADD` target `3c1948da` from `42000c50` in `<no function>`
  nearby:
  ```asm
  420002f0  beqz.n       a15, 0x42000325
  420002f2  s32i.n       a1, a12, 0xc
  420002f4  s32i.n       a9, a9, 0x24
  420002f6  s32i.n       a9, a9, 0x4
  420002f8  s32i.n       a9, a9, 0x24
  420002fa  s32i.n       a9, a9, 0x24
  42002b24  entry        a1, 0x20
  42002b27  l32r         a8, 0x41f40024
  42002b2a  movi.n       a9, 0x0
  42002b2c  s8i          a9, a8, 0x0
  ```
- string `3c1948e7`: `"LUT_FULL_ADD OK"`
- ref to `LUT_FULL_ADD` target `3c1948e7` from `00120c54` in `<no function>`
  nearby:
  ```asm
  00118802  l32i.n       a7, a8, 0x1c
  00118804  bltz         a5, 0x00118172
  00118807  call8        0x000af170
  0011880a  l32i.n       a8, a8, 0x1c
  0011880c  l32i.n       a7, a8, 0x1c
  0011880e  bany         0x001187a7, a7, a8
  00122b24  entry        a1, 0x20
  00122b27  l32r         a8, 0x00060024
  00122b2a  movi.n       a9, 0x0
  00122b2c  s8i          a9, a8, 0x0
  ```
- ref to `LUT_FULL_ADD` target `3c1948e7` from `42000c54` in `<no function>`
  nearby:
  ```asm
  420002f0  beqz.n       a15, 0x42000325
  420002f2  s32i.n       a1, a12, 0xc
  420002f4  s32i.n       a9, a9, 0x24
  420002f6  s32i.n       a9, a9, 0x4
  420002f8  s32i.n       a9, a9, 0x24
  420002fa  s32i.n       a9, a9, 0x24
  42002b24  entry        a1, 0x20
  42002b27  l32r         a8, 0x41f40024
  42002b2a  movi.n       a9, 0x0
  42002b2c  s8i          a9, a8, 0x0
  ```

Refs reported: 4

## `/lut.bin`

- string `00004896`: `"/lut.bin"`
- string `3c194896`: `"/lut.bin"`
- ref to `/lut.bin` target `3c194896` from `00120c38` in `<no function>`
  nearby:
  ```asm
  00118802  l32i.n       a7, a8, 0x1c
  00118804  bltz         a5, 0x00118172
  00118807  call8        0x000af170
  0011880a  l32i.n       a8, a8, 0x1c
  0011880c  l32i.n       a7, a8, 0x1c
  0011880e  bany         0x001187a7, a7, a8
  00122b24  entry        a1, 0x20
  00122b27  l32r         a8, 0x00060024
  00122b2a  movi.n       a9, 0x0
  00122b2c  s8i          a9, a8, 0x0
  ```
- ref to `/lut.bin` target `3c194896` from `42000c38` in `<no function>`
  nearby:
  ```asm
  420002f0  beqz.n       a15, 0x42000325
  420002f2  s32i.n       a1, a12, 0xc
  420002f4  s32i.n       a9, a9, 0x24
  420002f6  s32i.n       a9, a9, 0x4
  420002f8  s32i.n       a9, a9, 0x24
  420002fa  s32i.n       a9, a9, 0x24
  42002b24  entry        a1, 0x20
  42002b27  l32r         a8, 0x41f40024
  42002b2a  movi.n       a9, 0x0
  42002b2c  s8i          a9, a8, 0x0
  ```

Refs reported: 2

## `/lut_full.bin`

- string `0000489f`: `"/lut_full.bin"`
- string `3c19489f`: `"/lut_full.bin"`
- ref to `/lut_full.bin` target `3c19489f` from `00120c3c` in `<no function>`
  nearby:
  ```asm
  00118802  l32i.n       a7, a8, 0x1c
  00118804  bltz         a5, 0x00118172
  00118807  call8        0x000af170
  0011880a  l32i.n       a8, a8, 0x1c
  0011880c  l32i.n       a7, a8, 0x1c
  0011880e  bany         0x001187a7, a7, a8
  00122b24  entry        a1, 0x20
  00122b27  l32r         a8, 0x00060024
  00122b2a  movi.n       a9, 0x0
  00122b2c  s8i          a9, a8, 0x0
  ```
- ref to `/lut_full.bin` target `3c19489f` from `42000c3c` in `<no function>`
  nearby:
  ```asm
  420002f0  beqz.n       a15, 0x42000325
  420002f2  s32i.n       a1, a12, 0xc
  420002f4  s32i.n       a9, a9, 0x24
  420002f6  s32i.n       a9, a9, 0x4
  420002f8  s32i.n       a9, a9, 0x24
  420002fa  s32i.n       a9, a9, 0x24
  42002b24  entry        a1, 0x20
  42002b27  l32r         a8, 0x41f40024
  42002b2a  movi.n       a9, 0x0
  42002b2c  s8i          a9, a8, 0x0
  ```

Refs reported: 2

## `EPD BS`


Refs reported: 0

## `Refresh`

- string `0000ba53`: `"Refresh"`
- ref to `Refresh` target `0000ba53` from `0001f274` in `<no function>`
  nearby:
  ```asm
  0001f01f  ill          
  0001f020  bany         0x0001f024, a12, a3
  0001f023  ill          
  0001f024  l32i.n       a12, a9, 0x2c
  0001f026  sll          a0, a0
  0001f029  ret          
  000219d4  bnez.n       a0, 0x000219e4
  000219d6  movgez       a0, a0, a0
  000219d9  l8ui         a5, a0, 0x0
  000232e6  madd.s       f0, f0, f0
  ```
- ref to `Refresh` target `0000ba53` from `3c1af274` in `<no function>`
  nearby:
  ```asm
  002bca5d  wsr          a3, INTENABLE
  002bca60  rsync        
  002bca63  mov.n        a2, a4
  002bca65  retw.n       
  002be6f8  entry        a1, 0x20
  002be6fb  retw.n       
  40374000  s32e         a0, a5, -0x2000
  40374003  s32e         a1, a5, -0x4000
  40374006  s32e         a2, a5, -0x8000
  40374009  s32e         a3, a5, 0x0
  ```
- string `000e9291`: `"E (%u) %s: Unable to send Config Key Refresh Phase Status\n"`
- string `3c19ba53`: `"Refresh"`
- ref to `Refresh` target `3c19ba53` from `00118238` in `<no function>`
  nearby:
  ```asm
  0011800e  s32i.n       a1, a12, 0xc
  00118010  s32i.n       a13, a11, 0xc
  00118012  s32i.n       a1, a12, 0xc
  00118172  s32i.n       a1, a12, 0xc
  00118174  s32i.n       a8, a0, 0x10
  00118176  s32i.n       a1, a12, 0xc
  00118687  call8        0x000afff0
  0011868a  l32i.n       a8, a8, 0x1c
  0011868c  l32i.n       a7, a9, 0x18
  0011868e  l32i.n       a7, a7, 0x20
  ```
- ref to `Refresh` target `3c19ba53` from `3fc9ae68` in `<no function>`
  nearby:
  ```asm
  002bca5d  wsr          a3, INTENABLE
  002bca60  rsync        
  002bca63  mov.n        a2, a4
  002bca65  retw.n       
  002be6f8  entry        a1, 0x20
  002be6fb  retw.n       
  40374000  s32e         a0, a5, -0x2000
  40374003  s32e         a1, a5, -0x4000
  40374006  s32e         a2, a5, -0x8000
  40374009  s32e         a3, a5, 0x0
  ```
- string `3c279291`: `"E (%u) %s: Unable to send Config Key Refresh Phase Status\n"`

Refs reported: 4

