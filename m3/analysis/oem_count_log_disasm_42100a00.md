# Disassembly Report

## `0x42100a00` length `0x240`

```asm
42100a64  entry        a1, 0x40
42100a67  extui        a2, a2, 0x0, 0x8
42100a6a  beqi         a2, 0x1, 0x42100abe
42100a6d  beqi         a2, 0x2, 0x42100a73
42100a70  j            0x42100e8a
42100a73  l32r         a4, 0x4203dcac
42100a76  movi         a2, 0x0
42100a79  l32i.n       a3, a4, 0x0
42100a7b  l32r         a10, 0x4203e178
42100a7e  l32i         a3, a3, 0xbc
42100a81  callx8       a3
42100a84  add.n        a10, a10, a2
42100a86  movi.n       a3, 0x0
42100a88  memw         
42100a8b  s16i         a3, a10, 0x0
42100a8e  addi.n       a2, a2, 0x8
42100a90  movi         a3, 0x60
42100a93  bne          a2, a3, 0x42100a79
42100a96  l32r         a2, 0x4203dcb8
42100a99  l32i.n       a2, a2, 0x0
42100a9b  l32i         a2, a2, 0x278
42100a9e  callx8       a2
42100aa1  l32r         a3, 0x420409c4
42100aa4  l32r         a2, 0x420409c8
42100aa7  memw         
42100aaa  l32i.n       a4, a3, 0x0
42100aac  and          a2, a4, a2
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
42100ac8  l32i.n       a2, a2, 0x0
42100aca  addi.n       a11, a1, 0x4
42100acc  movi.n       a10, 0x31
42100ace  callx8       a2
42100ad1  l32r         a2, 0x4203dcac
42100ad4  bnez         a10, 0x42100b54
42100ad7  l8ui         a7, a1, 0x3
42100ada  l8ui         a3, a1, 0x2
42100add  slli         a7, a7, 0x18
42100ae0  l8ui         a5, a1, 0x1
42100ae3  l8ui         a4, a1, 0x0
42100ae6  bgez         a7, 0x42100af8
42100ae9  l32i         a6, a2, 0x0
42100aec  l32r         a12, 0x420409d4
42100aef  l32i         a6, a6, 0x8
42100af2  l32r         a11, 0x420409d8
42100af5  callx8       a6
42100af8  slli         a6, a3, 0x10
42100afb  l32r         a3, 0x4203edcc
42100afe  bnone        0x42100b10, a6, a3
42100b01  l32i.n       a3, a2, 0x0
42100b03  l32r         a12, 0x420409dc
42100b06  l32i.n       a3, a3, 0x8
42100b08  l32r         a11, 0x420409d8
42100b0b  movi.n       a10, 0x0
42100b0d  callx8       a3
42100b10  l32r         a3, 0x420409e0
42100b13  slli         a5, a5, 0x8
42100b16  bnone        0x42100b28, a5, a3
42100b19  l32i.n       a3, a2, 0x0
42100b1b  l32r         a12, 0x420409e4
42100b1e  l32i.n       a3, a3, 0x8
42100b20  l32r         a11, 0x420409d8
42100b23  movi.n       a10, 0x0
42100b25  callx8       a3
42100b28  movi         a3, -0x80
42100b2b  bnone        0x42100b3d, a4, a3
42100b2e  l32i.n       a3, a2, 0x0
42100b30  l32r         a12, 0x420409e8
42100b33  l32i.n       a3, a3, 0x8
42100b35  l32r         a11, 0x420409d8
42100b38  movi.n       a10, 0x0
42100b3a  callx8       a3
42100b3d  l32r         a3, 0x420409ec
42100b40  or           a3, a4, a3
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
42100b5f  srai         a3, a3, 0x1
42100b62  or           a3, a3, a4
42100b65  l32r         a4, 0x4203eb4c
42100b68  memw         
42100b6b  s32i.n       a3, a4, 0x0
42100b6d  l16ui        a3, a5, 0x2e
42100b70  l16ui        a4, a5, 0x30
42100b73  l32r         a5, 0x4203eb54
42100b76  slli         a3, a3, 0x1
42100b79  slli         a4, a4, 0x11
42100b7c  extui        a3, a3, 0x0, 0x10
42100b7f  bnone        0x42100b91, a4, a5
42100b82  l32i.n       a5, a2, 0x0
42100b84  l32r         a12, 0x420409f4
42100b87  l32i.n       a5, a5, 0x8
42100b89  l32r         a11, 0x420409d8
42100b8c  movi.n       a10, 0x0
42100b8e  callx8       a5
42100b91  movi         a5, -0x200
42100b94  bnone        0x42100ba8, a3, a5
42100b97  l32i         a5, a2, 0x0
42100b9a  l32r         a12, 0x420409f8
42100b9d  l32i.n       a5, a5, 0x8
42100b9f  l32r         a11, 0x420409d8
42100ba2  movi         a10, 0x0
42100ba5  callx8       a5
42100ba8  or           a3, a4, a3
42100bab  l32r         a4, 0x4203eb50
42100bae  memw         
42100bb1  s32i.n       a3, a4, 0x0
42100bb3  l32r         a3, 0x4203e384
42100bb6  l8ui         a4, a3, 0x45
42100bb9  l32r         a3, 0x420409bc
42100bbc  beqz.n       a4, 0x42100be9
42100bbe  l32r         a3, 0x420409fc
42100bc1  l32r         a4, 0x42040a00
42100bc4  memw         
42100bc7  l32i.n       a5, a3, 0x0
42100bc9  and          a5, a5, a4
42100bcc  l32r         a4, 0x42040a04
42100bcf  or           a4, a5, a4
42100bd2  memw         
42100bd5  s32i.n       a4, a3, 0x0
42100bd7  memw         
42100bda  l32i.n       a5, a3, 0x0
42100bdc  movi.n       a4, 0x1e
42100bde  or           a4, a5, a4
42100be1  memw         
42100be4  s32i.n       a4, a3, 0x0
42100be6  l32r         a3, 0x420409c0
42100be9  l32r         a4, 0x4203df88
42100bec  l32r         a5, 0x42040a18
42100bef  memw         
42100bf2  s32i.n       a3, a4, 0x0
42100bf4  l32r         a4, 0x42040a0c
42100bf7  l32r         a3, 0x42040a08
42100bfa  memw         
42100bfd  s32i.n       a4, a3, 0x0
42100bff  l32r         a4, 0x42040a14
42100c02  l32r         a3, 0x42040a10
42100c05  memw         
42100c08  s32i.n       a4, a3, 0x0
42100c0a  l32r         a3, 0x4203e178
42100c0d  memw         
42100c10  s32i.n       a3, a5, 0x0
42100c12  l32r         a5, 0x42040a1c
42100c15  memw         
42100c18  s32i.n       a4, a5, 0x0
42100c1a  l32r         a4, 0x42040a20
42100c1d  memw         
42100c20  s32i.n       a3, a4, 0x0
42100c22  l32r         a4, 0x42040a28
42100c25  l32r         a3, 0x42040a24
42100c28  memw         
42100c2b  s32i.n       a4, a3, 0x0
42100c2d  l32r         a4, 0x4203e174
42100c30  l32r         a3, 0x42040a2c
42100c33  memw         
42100c36  s32i.n       a4, a3, 0x0
42100c38  l32r         a3, 0x42040a30
42100c3b  movi.n       a4, 0x10
42100c3d  memw         
```

## `0x420409c0` length `0x80`

```asm
```

