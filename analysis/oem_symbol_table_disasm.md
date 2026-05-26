# Disassembly Report

## `0x42041000` length `0x1400`

```asm
42041000  addi.n       a8, a3, 0x8
```

## `0x4204315c` length `0x400`

```asm
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
42043200  add.n        a9, a4, a9
42043202  movi.n       a11, 0x1
42043204  bltu         a9, a4, 0x42043209
42043207  movi.n       a11, 0x0
42043209  add.n        a11, a11, a14
4204320b  bnez.n       a11, 0x42043214
4204320d  mov.n        a6, a9
4204320f  j            0x4204325d
42043214  l32r         a8, 0x41f82dac
42043217  callx8       a8
4204321a  mov.n        a14, a10
4204321c  l32r         a10, 0x41f80354
4204321f  s32i.n       a14, a1, 0x4
42043221  s32i         a11, a1, 0xc
42043224  l32r         a8, 0x41f80288
42043227  callx8       a8
4204322a  l32i.n       a14, a1, 0x4
4204322c  l32i.n       a15, a1, 0xc
4204322e  l32r         a12, 0x41f828dc
42043231  l32r         a13, 0x41f8241c
42043234  mov.n        a4, a10
42043236  mov.n        a11, a15
42043238  mov          a10, a14
4204323b  l32r         a8, 0x41f828fc
4204323e  callx8       a8
42043241  mov.n        a11, a10
42043243  l32r         a14, 0x41f80358
42043246  l32r         a10, 0x41f8035c
42043249  movi         a13, 0x179
4204324c  mov.n        a12, a4
4204324e  l32r         a8, 0x41f80290
42043251  callx8       a8
42043254  l8ui         a13, a3, 0x0
42043257  l8ui         a12, a3, 0x1
4204325a  l8ui         a10, a3, 0x2
4204325d  l32r         a9, 0x41f830a8
42043260  movi         a4, 0x66
42043263  s32i.n       a6, a9, 0x0
42043265  bne          a13, a4, 0x42043288
42043268  movi         a4, 0x74
4204326b  bne          a12, a4, 0x42043288
4204326e  movi         a4, 0x79
42043271  l8ui         a11, a3, 0x3
42043274  beq          a10, a4, 0x4204327a
42043277  j            0x42043ee4
4204327a  movi         a4, 0x70
4204327d  bne          a11, a4, 0x42043283
42043280  j            0x42043315
42043283  j            0x42043ee4
42043288  movi         a9, 0x66
4204328b  addi.n       a4, a3, 0x1
4204328d  beq          a12, a9, 0x42043298
42043290  l8ui         a11, a3, 0x3
42043293  j            0x420432bb
42043298  movi         a9, 0x74
4204329b  l8ui         a11, a3, 0x3
4204329e  beq          a10, a9, 0x420432a4
420432a1  j            0x420432bb
420432a4  movi         a9, 0x79
420432a7  beq          a11, a9, 0x420432ad
420432aa  j            0x420432ca
420432ad  l8ui         a9, a3, 0x4
420432b0  movi         a14, 0x70
420432b3  beq          a9, a14, 0x42043315
420432b6  j            0x420432e5
420432bb  movi         a9, 0x66
420432be  beq          a10, a9, 0x420432c4
420432c1  j            0x420432ca
420432c4  movi         a9, 0x74
420432c7  beq          a11, a9, 0x420432d0
420432ca  l8ui         a9, a3, 0x4
420432cd  j            0x420432e5
420432d0  l8ui         a9, a3, 0x4
420432d3  movi         a14, 0x79
420432d6  bne          a9, a14, 0x420432f1
420432d9  l8ui         a15, a3, 0x5
420432dc  movi         a14, 0x70
420432df  beq          a15, a14, 0x42043315
420432e2  j            0x420432f1
420432e5  movi         a14, 0x66
420432e8  bne          a11, a14, 0x420432f1
420432eb  movi         a14, 0x74
420432ee  beq          a9, a14, 0x42043315
420432f1  movi         a14, 0x66
420432f4  bne          a9, a14, 0x42043315
420432f7  l8ui         a14, a3, 0x5
420432fa  movi         a9, 0x74
420432fd  bne          a14, a9, 0x42043315
42043300  l8ui         a14, a3, 0x6
42043303  movi         a9, 0x79
42043306  bne          a14, a9, 0x42043315
42043309  l8ui         a14, a3, 0x7
4204330c  movi         a9, 0x70
4204330f  bne          a14, a9, 0x42043315
42043312  j            0x42043ed9
42043315  l32r         a8, 0x41f82dac
42043318  callx8       a8
4204331b  mov.n        a8, a10
4204331d  l32r         a10, 0x41f8024c
42043320  s32i.n       a8, a1, 0xc
42043322  s32i.n       a11, a1, 0x8
42043324  l32r         a8, 0x41f80288
42043327  callx8       a8
4204332a  l32i.n       a8, a1, 0xc
4204332c  l32i.n       a9, a1, 0x8
4204332e  l32r         a12, 0x41f828dc
42043331  l32r         a13, 0x41f8241c
42043334  mov.n        a2, a10
42043336  mov.n        a11, a9
42043338  mov.n        a10, a8
4204333a  l32r         a8, 0x41f828fc
4204333d  callx8       a8
42043340  mov.n        a11, a10
42043342  l32r         a14, 0x41f830bc
42043345  l32r         a10, 0x41f830c0
42043348  mov.n        a12, a2
4204334a  movi         a13, 0x7ee
4204334d  l32r         a8, 0x41f80290
42043350  callx8       a8
42043353  mov.n        a10, a5
42043355  call8        0x4203c068
4204335d  movi.n       a9, 0x34
4204335f  bne          a12, a9, 0x4204336a
42043362  movi.n       a9, 0x41
42043364  bne          a10, a9, 0x4204336a
42043367  beqi         a11, 0x20, 0x42043392
4204336a  movi.n       a10, 0x4d
4204336c  movi.n       a11, 0x34
4204336e  movi.n       a12, 0x41
42043370  l8ui         a9, a4, 0x0
42043373  bne          a9, a10, 0x42043388
42043376  l8ui         a9, a4, 0x1
42043379  bne          a9, a11, 0x42043388
4204337c  l8ui         a9, a4, 0x2
4204337f  bne          a9, a12, 0x42043388
42043382  l8ui         a9, a4, 0x3
42043385  beqi         a9, 0x20, 0x42043394
42043388  addi.n       a2, a2, 0x1
4204338a  addi.n       a4, a4, 0x1
4204338c  bnei         a2, 0x10, 0x42043370
4204338f  j            0x420433a4
42043392  movi.n       a2, 0x0
42043394  addi         a2, a2, -0x8
42043397  movi.n       a9, 0x1
42043399  movi.n       a4, 0x0
4204339b  movnez       a4, a9, a2
4204339e  extui        a2, a4, 0x0, 0x8
420433a1  j            0x420433a6
420433a4  movi.n       a2, 0x1
420433a6  l32r         a11, 0x41f830c4
420433a9  movi.n       a12, 0x14
420433ab  mov.n        a10, a3
420433ad  call8        0x4203b444
420433b0  l32r         a11, 0x41f830c8
420433b3  mov.n        a4, a10
420433b5  movi.n       a12, 0x14
420433b7  mov.n        a10, a3
420433b9  call8        0x4203b444
420433bc  movi.n       a9, 0x0
420433be  movi.n       a11, 0x1
420433c0  mov.n        a8, a9
420433c2  addi         a10, a10, -0x8
420433c5  addi         a4, a4, -0x8
420433c8  movnez       a8, a11, a4
420433cb  movnez       a9, a11, a10
420433ce  bnone        0x42043421, a8, a9
420433d1  beqz         a2, 0x42043421
420433d4  l32r         a8, 0x41f82dac
420433d7  callx8       a8
420433da  mov.n        a8, a10
420433dc  l32r         a10, 0x41f8024c
420433df  s32i.n       a8, a1, 0xc
420433e1  s32i.n       a11, a1, 0x8
420433e3  l32r         a8, 0x41f80288
420433e6  callx8       a8
420433e9  l32i.n       a8, a1, 0xc
420433eb  l32i.n       a9, a1, 0x8
420433ed  l32r         a12, 0x41f828dc
420433f0  l32r         a13, 0x41f8241c
420433f3  mov.n        a2, a10
420433f5  mov.n        a11, a9
420433f7  mov          a10, a8
420433fa  l32r         a8, 0x41f828fc
420433fd  callx8       a8
42043400  mov.n        a11, a10
42043402  l32r         a14, 0x41f830bc
42043405  addi.n       a3, a3, 0x8
42043407  l32r         a10, 0x41f830cc
4204340a  mov.n        a12, a2
4204340c  mov.n        a15, a3
4204340e  movi         a13, 0x7f7
42043411  l32r         a8, 0x41f80290
42043414  callx8       a8
42043417  mov.n        a10, a5
42043419  call8        0x4203c068
42043421  movi.n       a2, 0x2
42043423  s32i         a2, a5, 0x250
42043426  l32r         a2, 0x41f830a0
42043429  s32i.n       a6, a7, 0x0
4204342b  s32i.n       a6, a2, 0x0
4204342d  movi.n       a2, 0x0
4204342f  j            0x42043ee9
42043434  beqi         a2, 0x2, 0x4204343a
42043437  j            0x42043514
4204343a  l8ui         a4, a3, 0x0
4204343d  l8ui         a2, a3, 0x1
42043440  slli         a4, a4, 0x18
42043443  slli         a2, a2, 0x10
42043446  add          a2, a4, a2
42043449  srai         a10, a4, 0x1f
4204344c  movi         a9, 0x1
4204344f  bltu         a2, a4, 0x42043454
42043452  mov.n        a9, a6
42043454  l8ui         a4, a3, 0x3
42043457  add.n        a10, a9, a10
42043459  add          a4, a2, a4
4204345c  movi.n       a9, 0x1
4204345e  bltu         a4, a2, 0x42043464
42043461  movi         a9, 0x0
42043464  l8ui         a2, a3, 0x2
42043467  add.n        a10, a9, a10
42043469  slli         a2, a2, 0x8
4204346c  add.n        a2, a4, a2
4204346e  movi.n       a9, 0x1
42043470  bltu         a2, a4, 0x42043475
42043473  movi.n       a9, 0x0
42043475  add.n        a9, a9, a10
42043477  bnez.n       a9, 0x42043480
42043479  mov.n        a6, a2
4204347b  j            0x420434bf
42043480  l32r         a8, 0x41f82dac
42043483  callx8       a8
42043486  mov.n        a14, a10
42043488  l32r         a10, 0x41f80354
4204348b  s32i.n       a14, a1, 0x4
4204348d  s32i.n       a11, a1, 0xc
4204348f  l32r         a8, 0x41f80288
42043492  callx8       a8
42043495  l32i.n       a14, a1, 0x4
42043497  l32i.n       a15, a1, 0xc
42043499  l32r         a12, 0x41f828dc
4204349c  l32r         a13, 0x41f8241c
4204349f  mov.n        a2, a10
420434a1  mov.n        a11, a15
420434a3  mov          a10, a14
420434a6  l32r         a8, 0x41f828fc
420434a9  callx8       a8
420434ac  mov.n        a11, a10
420434ae  l32r         a14, 0x41f80358
420434b1  l32r         a10, 0x41f8035c
420434b4  movi         a13, 0x179
420434b7  mov.n        a12, a2
420434b9  l32r         a8, 0x41f80290
420434bc  callx8       a8
420434bf  l32r         a2, 0x41f830a8
420434c2  l32r         a11, 0x41f80340
420434c5  movi.n       a12, 0xa
420434c7  mov          a10, a3
420434ca  s32i         a6, a2, 0x0
420434cd  call8        0x4203b444
420434d0  bnei         a10, 0x4, 0x420434d8
420434d3  j            0x42043d50
420434d8  l32r         a11, 0x41f830d0
420434db  movi.n       a12, 0xa
420434dd  mov.n        a10, a3
420434df  call8        0x4203b444
420434e2  bnei         a10, 0x4, 0x420434e8
420434e5  j            0x42043504
420434e8  l32r         a11, 0x41f830d4
420434eb  movi         a12, 0xa
420434ee  mov          a10, a3
420434f1  call8        0x4203b444
420434f4  bnei         a10, 0x4, 0x42043504
420434f7  movi         a2, 0x6
420434fa  s32i         a2, a5, 0x250
420434fd  movi.n       a2, 0x0
420434ff  j            0x42043ee9
42043504  l32r         a4, 0x41f830a0
42043507  s32i.n       a6, a7, 0x0
42043509  l32i.n       a3, a4, 0x0
4204350b  movi.n       a2, 0x0
4204350d  add.n        a8, a3, a6
4204350f  s32i.n       a8, a4, 0x0
42043511  j            0x42043ee9
42043514  beqi         a2, 0x3, 0x4204351a
42043517  j            0x420435d9
4204351a  l32r         a14, 0x41f830a8
4204351d  l32r         a13, 0x41f830a0
42043520  l32i.n       a6, a14, 0x0
42043522  l32i.n       a2, a13, 0x0
42043524  bltui        a4, 0x4, 0x4204359b
42043527  addi         a11, a4, -0x4
4204352a  beqz         a11, 0x4204359b
4204352d  l8ui         a10, a3, 0x0
42043530  movi         a8, 0x74
42043533  bne          a10, a8, 0x42043551
42043536  l8ui         a10, a3, 0x1
42043539  movi         a8, 0x72
4204353c  bne          a10, a8, 0x42043551
4204353f  l8ui         a10, a3, 0x2
42043542  movi         a8, 0x61
42043545  bne          a10, a8, 0x42043551
42043548  l8ui         a10, a3, 0x3
4204354b  movi         a8, 0x6b
4204354e  beq          a10, a8, 0x4204359b
42043551  beqi         a11, 0x1, 0x4204359b
42043554  addi.n       a10, a3, 0x1
42043556  movi         a8, 0x1
42043559  movi         a15, 0x74
```

## `0x4210a000` length `0x1200`

```asm
4210a000  l32i.n       a1, a2, 0x2c
4210a002  j            0x420fe10e
4210aec4  entry        a1, 0x20
4210aec7  movi         a4, 0x100
4210aeca  bnez.n       a2, 0x4210aece
4210aecc  retw.n       
4210aece  l32i.n       a8, a2, 0xc
4210aed0  l32i.n       a5, a2, 0x0
4210aed2  l32r         a3, 0x4204aebc
4210aed5  bany         0x4210aee0, a4, a8
4210aed8  l32i.n       a10, a2, 0x8
4210aeda  bnez.n       a10, 0x4210aefd
4210aedc  l32i.n       a10, a2, 0x10
4210aede  bnez.n       a10, 0x4210af08
4210aee0  l32i.n       a8, a2, 0xc
4210aee2  bbsi         a8, 0x9, 0x4210aeee
4210aee5  l32i.n       a10, a2, 0x20
4210aee7  beqz.n       a10, 0x4210aeee
4210aee9  l32i.n       a8, a3, 0x4
4210aeeb  callx8       a8
4210aeee  l32i.n       a3, a3, 0x4
4210aef0  mov.n        a10, a2
4210aef2  callx8       a3
4210aef5  mov.n        a2, a5
4210aef7  j            0x4210aeca
4210aefd  call8        0x4210aec4
4210af00  l32i.n       a8, a2, 0xc
4210af02  bnone        0x4210aedc, a4, a8
4210af05  j            0x4210aee0
4210af08  l32i.n       a8, a3, 0x4
4210af0a  callx8       a8
4210af0d  j            0x4210aee0
4210af10  entry        a1, 0x20
4210af13  l32r         a8, 0x42049c38
4210af16  movi         a12, 0x2
4210af19  l32i         a11, a8, 0x8
4210af1c  l32i         a9, a8, 0x38
4210af1f  s8i          a12, a8, 0x30
4210af22  sub          a9, a11, a9
4210af25  l8ui         a12, a8, 0x3c
4210af28  s8i          a3, a8, 0x2c
4210af2b  s32i.n       a9, a8, 0x28
4210af2d  mov.n        a10, a2
4210af2f  bnez         a12, 0x4210af3b
4210af32  l32r         a2, 0x4204aec0
4210af35  s8i          a12, a8, 0x3e
4210af38  s16i         a2, a8, 0x3c
4210af3b  beqz.n       a11, 0x4210af4d
4210af3d  beqz.n       a9, 0x4210af4d
4210af3f  l8ui         a9, a8, 0xc
4210af42  srli         a9, a9, 0x7
4210af45  beqz.n       a9, 0x4210af50
4210af47  l8ui         a11, a8, 0x2e
4210af4a  call8        0x4210bdf4
4210af4d  retw.n       
4210af50  l8ui         a11, a8, 0x2f
4210af53  j            0x4210af4a
4210af58  entry        a1, 0x20
4210af5b  l32r         a8, 0x42049c34
4210af5e  l32r         a3, 0x42049c38
4210af61  beqz         a8, 0x4210af85
4210af64  l8ui         a10, a3, 0xd
4210af67  l32r         a8, 0x42049c34
4210af6a  callx8       a8
4210af6d  bnez         a10, 0x4210af85
4210af70  l32r         a4, 0x42049c3c
4210af73  s8i          a10, a3, 0x3e
4210af76  s16i         a4, a3, 0x3c
4210af79  movi         a11, 0x1
4210af7c  mov.n        a10, a2
4210af7e  call8        0x4210af10
4210af81  retw.n       
4210af85  l32i         a13, a3, 0x38
4210af88  l32i         a8, a3, 0x8
4210af8b  l32r         a12, 0x42049c40
4210af8e  sub          a8, a8, a13
4210af91  l32r         a13, 0x4203f880
4210af94  l8ui         a11, a3, 0x2f
4210af97  minu         a13, a8, a13
4210af9a  mov          a10, a2
4210af9d  call8        0x4210bd60
4210afa0  j            0x4210af81
4210afa4  entry        a1, 0x30
4210afa7  l32r         a4, 0x42049c38
4210afaa  l16ui        a5, a4, 0x16
4210afad  slli         a3, a5, 0x8
4210afb0  srli         a5, a5, 0x8
4210afb3  or           a5, a3, a5
4210afb6  extui        a3, a5, 0x0, 0x10
4210afb9  l32i.n       a5, a4, 0x8
4210afbb  beqz.n       a3, 0x4210afc3
4210afbd  quou         a3, a5, a3
4210afc0  extui        a3, a3, 0x0, 0x10
4210afc3  l32i         a6, a4, 0x10
4210afc6  l8ui         a10, a4, 0x14
4210afc9  srli         a6, a6, 0x8
4210afcc  slli         a10, a10, 0x18
4210afcf  or           a10, a10, a6
4210afd2  l32r         a8, 0x42049c48
4210afd5  callx8       a8
4210afd8  l32i.n       a12, a4, 0x38
4210afda  l32r         a6, 0x42049c40
4210afdd  quou         a11, a12, a3
4210afe0  l32r         a14, 0x4203f880
4210afe3  sub          a5, a5, a12
4210afe6  add.n        a11, a11, a10
4210afe8  l8ui         a10, a4, 0xd
4210afeb  remu         a12, a12, a3
4210afee  minu         a14, a5, a14
4210aff1  mov.n        a13, a6
4210aff3  l32r         a8, 0x42049c4c
4210aff6  callx8       a8
4210aff9  mov.n        a3, a10
4210affb  bgez         a10, 0x4210b014
4210affe  l32r         a3, 0x42049c44
4210b001  movi.n       a11, 0x1
4210b003  s16i         a3, a4, 0x3c
4210b006  mov.n        a10, a2
4210b008  movi.n       a3, 0x0
4210b00a  s8i          a3, a4, 0x3e
4210b00d  call8        0x4210af10
4210b010  retw.n       
4210b014  l8ui         a4, a4, 0x2e
4210b017  bnez         a10, 0x4210b041
4210b01a  movi         a12, 0x6
4210b01d  mov          a11, a10
4210b020  add.n        a10, a1, a12
4210b022  l32r         a8, 0x4203e38c
4210b025  callx8       a8
4210b028  mov.n        a11, a3
4210b02a  s8i          a2, a1, 0x4
4210b02d  addi.n       a10, a1, 0x4
4210b02f  movi.n       a2, 0x7
4210b031  s8i          a2, a1, 0x5
4210b034  s8i          a4, a1, 0x8
4210b037  s32i.n       a3, a1, 0xc
4210b039  call8        0x4210bad8
4210b03c  j            0x4210b010
4210b041  extui        a13, a10, 0x0, 0x10
4210b044  mov          a12, a6
4210b047  mov          a11, a4
4210b04a  mov          a10, a2
4210b04d  call8        0x4210bd60
4210b050  j            0x4210b010
4210b068  entry        a1, 0x20
4210b06b  l32r         a10, 0x42049c38
4210b06e  movi.n       a12, 0x40
4210b070  movi.n       a11, 0x0
4210b072  l32r         a8, 0x4203e38c
4210b075  callx8       a8
4210b078  retw.n       
4210b07c  entry        a1, 0x20
4210b07f  l32r         a10, 0x42049c38
4210b082  movi.n       a12, 0x40
4210b084  movi.n       a11, 0x0
4210b086  l32r         a8, 0x4203e38c
4210b089  callx8       a8
4210b08c  retw.n       
4210b090  entry        a1, 0x20
4210b093  l8ui         a5, a3, 0x5
4210b096  extui        a2, a2, 0x0, 0x8
4210b099  extui        a4, a4, 0x0, 0x10
4210b09c  beqi         a5, 0x8, 0x4210b0a5
4210b09f  movi.n       a2, 0x0
4210b0a1  retw.n       
4210b0a5  l8ui         a5, a3, 0x6
4210b0a8  bnei         a5, 0x6, 0x4210b09f
4210b0ab  l8ui         a5, a3, 0x7
4210b0ae  movi.n       a9, 0x1
4210b0b0  addi         a5, a5, -0x50
4210b0b3  movi.n       a8, 0x0
4210b0b5  movnez       a8, a9, a5
4210b0b8  extui        a5, a8, 0x0, 0x8
4210b0bb  bnez         a5, 0x4210b09f
4210b0be  movi.n       a8, 0x16
4210b0c0  bgeu         a8, a4, 0x4210b09f
4210b0c3  l32r         a4, 0x42049c38
4210b0c6  l8ui         a8, a3, 0x2
4210b0c9  movi.n       a13, 0x2
4210b0cb  s8i          a8, a4, 0x2d
4210b0ce  l8ui         a11, a3, 0x0
4210b0d1  l32r         a15, 0x42049c50
4210b0d4  l32r         a14, 0x42049c54
4210b0d7  mov.n        a12, a13
4210b0d9  add.n        a11, a3, a11
4210b0db  mov.n        a10, a2
4210b0dd  call8        0x4210bcb4
4210b0e0  beqz         a10, 0x4210b09f
4210b0e3  l8ui         a11, a4, 0x2f
4210b0e6  mov.n        a10, a2
4210b0e8  movi.n       a13, 0x1f
4210b0ea  mov.n        a12, a4
4210b0ec  s8i          a5, a4, 0x30
4210b0ef  movi.n       a2, 0x17
4210b0f1  call8        0x4210bd60
4210b0f4  j            0x4210b0a1
4210b0f8  entry        a1, 0x30
4210b0fb  extui        a3, a3, 0x0, 0x8
4210b0fe  extui        a5, a2, 0x0, 0x8
4210b101  beqi         a3, 0x1, 0x4210b109
4210b104  movi.n       a10, 0x1
4210b106  j            0x4210b168
4210b109  l8ui         a11, a4, 0x0
4210b10c  extui        a2, a11, 0x0, 0x7
4210b10f  beqi         a2, 0x2, 0x4210b115
4210b112  j            0x4210b1ac
4210b115  l8ui         a2, a4, 0x1
4210b118  beqi         a2, 0x1, 0x4210b11e
4210b11b  j            0x4210b1ac
4210b11e  l8ui         a10, a4, 0x3
4210b121  l8ui         a9, a4, 0x2
4210b124  slli         a10, a10, 0x8
4210b127  or           a9, a10, a9
4210b12a  bnez         a9, 0x4210b1ac
4210b12d  l8ui         a8, a4, 0x4
4210b130  l32r         a4, 0x42049c38
4210b133  l8ui         a10, a4, 0x30
4210b136  bnei         a10, 0x4, 0x4210b144
4210b139  mov.n        a11, a8
4210b13b  mov.n        a10, a5
4210b13d  call8        0x4210bdf4
4210b140  j            0x4210b104
4210b144  l8ui         a11, a4, 0x2e
4210b147  bne          a11, a8, 0x4210b16c
4210b14a  bnei         a10, 0x2, 0x4210b104
4210b14d  l32i         a2, a4, 0x8
4210b150  l32i         a3, a4, 0x38
4210b153  l32r         a12, 0x42049c58
4210b156  sub          a2, a2, a3
4210b159  s32i         a2, a4, 0x28
4210b15c  movi.n       a2, 0x3
4210b15e  s8i          a2, a4, 0x30
4210b161  movi.n       a13, 0xd
4210b163  mov.n        a10, a5
4210b165  call8        0x4210bd60
4210b168  mov.n        a2, a10
4210b16a  retw.n       
4210b16c  mov          a6, a9
4210b16f  moveqz       a6, a2, a10
4210b172  extui        a10, a6, 0x0, 0x8
4210b175  l8ui         a3, a4, 0x2f
4210b178  beqz         a10, 0x4210b104
4210b17b  sub          a10, a3, a8
4210b17e  moveqz       a9, a2, a10
4210b181  extui        a10, a9, 0x0, 0x8
4210b184  beqz         a10, 0x4210b104
4210b187  mov          a11, a3
4210b18a  mov.n        a10, a5
4210b18c  call8        0x4210bdd0
4210b18f  bnez         a10, 0x4210b104
4210b192  mov.n        a11, a3
4210b194  mov.n        a10, a5
4210b196  call8        0x4210c594
4210b199  bnez         a10, 0x4210b104
4210b19c  movi.n       a2, 0x0
4210b19e  s8i          a2, a4, 0x30
4210b1a1  movi.n       a13, 0x1f
4210b1a3  mov.n        a12, a4
4210b1a5  l8ui         a11, a4, 0x2f
4210b1a8  j            0x4210b163
4210b1ac  movi         a2, 0x60
4210b1af  and          a11, a11, a2
4210b1b2  movi         a10, 0x0
4210b1b5  bnei         a11, 0x20, 0x4210b168
4210b1b8  l8ui         a2, a4, 0x1
4210b1bb  movi         a3, 0xfe
4210b1be  beq          a2, a3, 0x4210b205
4210b1c1  movi         a3, 0xff
4210b1c4  bne          a2, a3, 0x4210b168
4210b1c7  l8ui         a2, a4, 0x3
4210b1ca  l8ui         a3, a4, 0x2
4210b1cd  slli         a2, a2, 0x8
4210b1d0  or           a2, a2, a3
4210b1d3  bne          a2, a10, 0x4210b168
4210b1d6  l8ui         a2, a4, 0x7
4210b1d9  l8ui         a3, a4, 0x6
4210b1dc  slli         a2, a2, 0x8
4210b1df  or           a2, a2, a3
4210b1e2  beq          a2, a10, 0x4210b1e8
4210b1e5  j            0x4210b168
4210b1e8  l32r         a2, 0x42049c38
4210b1eb  mov          a11, a4
4210b1ee  s8i          a10, a2, 0x30
4210b1f1  s32i         a10, a2, 0x34
4210b1f4  s32i.n       a10, a2, 0x38
4210b1f6  s16i         a10, a2, 0x3c
4210b1f9  s8i          a10, a2, 0x3e
4210b1fc  mov.n        a10, a5
4210b1fe  call8        0x4210b7e4
```

