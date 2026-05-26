# Hardware Callsite Contexts

## `>>>Count:%d Time:%dms`

- string `00001464`: `">>>Count:%d Time:%dms\n"`
- ref to `>>>Count:%d Time:%dms` target `00001464` from `42100abc` in `<no function>`
  nearby:
  ```asm
  42100aaf  memw         
  42100ab2  s32i.n       a2, a3, 0x0
  42100ab4  l32r         a2, 0x420409cc
  42100ab7  movi.n       a3, 0x0
  42100ab9  memw         
  42100abc  s32i.n       a3, a2, 0x0
  42100abe  movi.n       a2, 0x4
  42100ac0  s8i          a2, a1, 0x4
  42100ac3  l32r         a2, 0x420409d0
  42100ac6  mov.n        a12, a1
  ```
- ref to `>>>Count:%d Time:%dms` target `00001464` from `42100b52` in `<no function>`
  nearby:
  ```asm
  42100b43  or           a4, a3, a7
  42100b46  or           a3, a4, a6
  42100b49  l32r         a4, 0x420409cc
  42100b4c  or           a3, a3, a5
  42100b4f  memw         
  42100b52  s32i.n       a3, a4, 0x0
  42100b54  l32r         a5, 0x4203e384
  42100b57  l32r         a4, 0x420409f0
  42100b5a  l8ui         a3, a5, 0x1a
  42100b5d  addi.n       a3, a3, 0x1
  ```
- ref to `>>>Count:%d Time:%dms` target `00001464` from `420409cc` in `<no function>`
  nearby:
  ```asm
  4204017e  s32i         a3, a2, 0x2b8
  42040181  s32i         a3, a2, 0x2cc
  42040184  j            0x4204018a
  42040188  mov.n        a4, a8
  4204018a  mov.n        a2, a4
  4204018c  retw.n       
  4204315c  entry        a1, 0x1a0
  4204315f  mov.n        a5, a2
  42043161  l32r         a2, 0x41f82fcc
  42043164  memw         
  ```
- string `3c191464`: `">>>Count:%d Time:%dms\n"`

Refs reported: 3

## `#ClickLock`

- string `00002b1f`: `"#ClickLock\r"`
- string `3c192b1f`: `"#ClickLock\r"`

Refs reported: 0

## `TRACE`

- string `000a6a8b`: `"TRACE"`
- string `3c236a8b`: `"TRACE"`
- ref to `TRACE` target `3c236a8b` from `001185c8` in `<no function>`
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
- ref to `TRACE` target `3c236a8b` from `3fc9b1f8` in `<no function>`
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

## `ERROR: %s%s`

- string `000a9220`: `"ERROR: %s%s"`
- string `3c239220`: `"ERROR: %s%s"`
- ref to `ERROR: %s%s` target `3c239220` from `00160784` in `<no function>`
  nearby:
  ```asm
  0016017e  s32i         a3, a2, 0x2b8
  00160181  s32i         a3, a2, 0x2cc
  00160184  j            0x0016018a
  00160188  mov.n        a4, a8
  0016018a  mov.n        a2, a4
  0016018c  retw.n       
  0016315c  entry        a1, 0x1a0
  0016315f  mov.n        a5, a2
  00163161  l32r         a2, 0x000a2fcc
  00163164  memw         
  ```
- ref to `ERROR: %s%s` target `3c239220` from `42040784` in `<no function>`
  nearby:
  ```asm
  4204017e  s32i         a3, a2, 0x2b8
  42040181  s32i         a3, a2, 0x2cc
  42040184  j            0x4204018a
  42040188  mov.n        a4, a8
  4204018a  mov.n        a2, a4
  4204018c  retw.n       
  4204315c  entry        a1, 0x1a0
  4204315f  mov.n        a5, a2
  42043161  l32r         a2, 0x41f82fcc
  42043164  memw         
  ```

Refs reported: 2

## `INFO: %s%s`

- string `000a922c`: `"INFO: %s%s"`
- string `3c23922c`: `"INFO: %s%s"`

Refs reported: 0

## `esp_log_level_set`

- string `000d3cb8`: `"esp_log_level_set"`
- string `3c263cb8`: `"esp_log_level_set"`

Refs reported: 0

## `get_cached_log_level`

- string `000d3ca3`: `"get_cached_log_level"`
- string `3c263ca3`: `"get_cached_log_level"`

Refs reported: 0

