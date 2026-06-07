# Disassembly Report

## `0x42000800` length `0x800`

```asm
42000800  and          a1, a0, a3
42000803  beqz.n       a3, 0x42000829
42000805  l8ui         a1, a7, 0x11
```

## `0x42000c00` length `0x200`

```asm
42000c00  beqi         a8, 0x4, 0x42000c1d
42000c03  movi.n       a12, 0x3d
42000c05  bnall        0x42000c48, a10, a6
42000c08  bnall        0x42000c34, a8, a14
42000c0b  movi.n       a0, 0x33
42000c0d  l32i.n       a4, a9, 0x4
42000c0f  movi.n       a4, 0x3f
```

## `0x42043000` length `0x200`

```asm
42043000  float.s      f6, a11, 0x1
4204315c  entry        a1, 0x1a0
4204315f  mov.n        a5, a2
42043161  l32r         a2, 0x41f82fcc
42043164  memw         
42043167  l32i.n       a7, a2, 0x0
42043169  l32i         a2, a5, 0x250
4204316c  memw         
4204316f  s32i         a7, a1, 0x17c
42043172  beqz.n       a2, 0x42043177
42043174  j            0x42043e71
42043177  l32r         a3, 0x41f830a0
4204317a  s32i.n       a2, a3, 0x0
4204317c  l32r         a3, 0x41f830a4
4204317f  s32i.n       a2, a3, 0x0
42043181  l32r         a3, 0x41f830a8
42043184  s32i.n       a2, a3, 0x0
42043186  l32r         a3, 0x41f830ac
42043189  s32i.n       a2, a3, 0x0
4204318b  l32r         a3, 0x41f830b0
4204318e  s32i.n       a2, a3, 0x0
42043190  l32r         a3, 0x41f830b4
42043193  s32i.n       a2, a3, 0x0
42043195  movi.n       a3, 0x1
42043197  s32i         a3, a5, 0x250
4204319a  j            0x42043ee9
4204319d  l16ui        a2, a5, 0x60
420431a0  minu         a2, a2, a4
420431a3  bgeu         a2, a6, 0x420431b8
420431a6  l32r         a4, 0x41f830b8
420431a9  sub          a8, a6, a2
420431ac  l32i.n       a3, a4, 0x0
420431ae  s32i.n       a8, a7, 0x0
420431b0  add.n        a3, a3, a2
420431b2  s32i.n       a3, a4, 0x0
420431b4  j            0x42043ee9
420431b8  l32r         a3, 0x41f830b8
420431bb  movi.n       a2, 0x0
420431bd  s32i.n       a2, a7, 0x0
420431bf  s32i.n       a2, a3, 0x0
420431c1  mov.n        a2, a6
420431c3  j            0x42043ee9
420431c8  beqi         a2, 0x1, 0x420431ce
420431cb  j            0x42043434
420431ce  l8ui         a13, a3, 0x0
420431d1  l8ui         a12, a3, 0x1
420431d4  slli         a4, a13, 0x18
420431d7  slli         a9, a12, 0x10
420431da  add          a9, a4, a9
420431dd  srai         a14, a4, 0x1f
420431e0  mov          a11, a2
420431e3  bltu         a9, a4, 0x420431e8
420431e6  mov.n        a11, a6
420431e8  l8ui         a4, a3, 0x3
420431eb  add.n        a14, a11, a14
420431ed  add          a4, a9, a4
420431f0  movi.n       a11, 0x1
420431f2  bltu         a4, a9, 0x420431f8
420431f5  movi         a11, 0x0
420431f8  l8ui         a10, a3, 0x2
420431fb  add.n        a14, a11, a14
420431fd  slli         a9, a10, 0x8
```

