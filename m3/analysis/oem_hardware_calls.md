# Hardware Callsite Contexts

## `gpio_set_level`

- string `000c9ea5`: `"gpio_set_level"`
- string `3c259ea5`: `"gpio_set_level"`
- ref to `gpio_set_level` target `3c259ea5` from `00161ab8` in `<no function>`
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
- ref to `gpio_set_level` target `3c259ea5` from `42041ab8` in `<no function>`
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

## `gpio_config`

- string `000c9e08`: `"gpio_config"`
- string `000f4c03`: `"E (%u) %s: gpio_config (interrupt) failed with rc=0x%x\n"`
- string `3c259e08`: `"gpio_config"`
- ref to `gpio_config` target `3c259e08` from `00161ad8` in `<no function>`
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
- ref to `gpio_config` target `3c259e08` from `42041ad8` in `<no function>`
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
- string `3c284c03`: `"E (%u) %s: gpio_config (interrupt) failed with rc=0x%x\n"`

Refs reported: 2

## `gpio_reset_pin`

- string `000c9df9`: `"gpio_reset_pin"`
- string `3c259df9`: `"gpio_reset_pin"`
- ref to `gpio_reset_pin` target `3c259df9` from `00161ae4` in `<no function>`
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
- ref to `gpio_reset_pin` target `3c259df9` from `42041ae4` in `<no function>`
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

## `__pinMode`

- string `000c7f90`: `"__pinMode"`
- string `3c257f90`: `"__pinMode"`

Refs reported: 0

## `digitalWrite`


Refs reported: 0

## `pinMode`

- string `000c7f90`: `"__pinMode"`
- string `3c257f90`: `"__pinMode"`

Refs reported: 0

## `spi_bus_initialize`

- string `000cbe12`: `"spi_bus_initialize"`
- string `000f4ce0`: `"E (%u) %s: spi_bus_initialize failed with rc=0x%x\n"`
- string `3c25be12`: `"spi_bus_initialize"`
- ref to `spi_bus_initialize` target `3c25be12` from `001620d4` in `<no function>`
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
- ref to `spi_bus_initialize` target `3c25be12` from `420420d4` in `<no function>`
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
- string `3c284ce0`: `"E (%u) %s: spi_bus_initialize failed with rc=0x%x\n"`

Refs reported: 2

## `spi_bus_add_device`

- string `000f55e9`: `"spi_bus_add_device"`
- string `3c2855e9`: `"spi_bus_add_device"`
- ref to `spi_bus_add_device` target `3c2855e9` from `0022a0bc` in `<no function>`
  nearby:
  ```asm
  00229790  l16ui        a13, a2, 0x8
  00229793  l16ui        a12, a2, 0x6
  00229796  addi.n       a11, a2, 0xc
  00229798  mov          a1, a1
  0022979b  call8        0x00224bd0
  0022979e  retw.n       
  0022b068  entry        a1, 0x20
  0022b06b  l32r         a10, 0x00169c38
  0022b06e  movi.n       a12, 0x40
  0022b070  movi.n       a11, 0x0
  ```
- ref to `spi_bus_add_device` target `3c2855e9` from `4210a0bc` in `<no function>`
  nearby:
  ```asm
  42109b2d  callx8       a8
  42109b30  l32i.n       a2, a2, 0x0
  42109b32  movi.n       a10, 0x0
  42109b34  l32i         a2, a2, 0xac
  42109b37  callx8       a2
  42109b3a  retw.n       
  4210aec4  entry        a1, 0x20
  4210aec7  movi         a4, 0x100
  4210aeca  bnez.n       a2, 0x4210aece
  4210aecc  retw.n       
  ```

Refs reported: 2

## `i2c_param_config`

- string `000c813e`: `"[%6u][E][%s:%u] %s(): i2c_param_config failed\r\n"`
- string `000ca4a0`: `"i2c_param_config"`
- string `3c25813e`: `"[%6u][E][%s:%u] %s(): i2c_param_config failed\r\n"`
- ref to `i2c_param_config` target `3c25813e` from `001616fc` in `<no function>`
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
- ref to `i2c_param_config` target `3c25813e` from `420416fc` in `<no function>`
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
- string `3c25a4a0`: `"i2c_param_config"`
- ref to `i2c_param_config` target `3c25a4a0` from `00161c00` in `<no function>`
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
- ref to `i2c_param_config` target `3c25a4a0` from `42041c00` in `<no function>`
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

Refs reported: 4

## `ledc_channel_config`

- string `000cb42b`: `"ledc_channel_config"`
- string `3c25b42b`: `"ledc_channel_config"`
- ref to `ledc_channel_config` target `3c25b42b` from `00161ea4` in `<no function>`
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
- ref to `ledc_channel_config` target `3c25b42b` from `42041ea4` in `<no function>`
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

## `ledc_timer_config`

- string `000cb45a`: `"ledc_timer_config"`
- string `3c25b45a`: `"ledc_timer_config"`
- ref to `ledc_timer_config` target `3c25b45a` from `00161e80` in `<no function>`
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
- ref to `ledc_timer_config` target `3c25b45a` from `42041e80` in `<no function>`
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

## `i2s_set_pin`

- string `000cb09f`: `"i2s_set_pin"`
- string `3c25b09f`: `"i2s_set_pin"`
- ref to `i2s_set_pin` target `3c25b09f` from `00161cc8` in `<no function>`
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
- ref to `i2s_set_pin` target `3c25b09f` from `42041cc8` in `<no function>`
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

## `i2s_driver_install`

- string `000caf8a`: `"i2s_driver_install"`
- string `3c25af8a`: `"i2s_driver_install"`
- ref to `i2s_driver_install` target `3c25af8a` from `00161da4` in `<no function>`
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
- ref to `i2s_driver_install` target `3c25af8a` from `42041da4` in `<no function>`
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

## `i2s_write`

- string `000caef1`: `"i2s_write"`
- string `3c25aef1`: `"i2s_write"`

Refs reported: 0

