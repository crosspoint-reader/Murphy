# Function Reference Report

## Target `FUN_42038810` at `42038810`

Reference count: 1

- `UNCONDITIONAL_CALL` from `42022618` in `<none>`

```asm
42022600  s8i          a10, a8, 0x21
42022603  s32i.n       a9, a8, 0x4
42022605  add.n        a10, a1, a14
42022607  s32i         a9, a8, 0x40
4202260a  movi.n       a13, 0x7
4202260c  movi.n       a12, 0x6
4202260e  movi.n       a11, 0x5
42022610  s32i.n       a9, a8, 0xc
42022612  s8i          a9, a8, 0x22
42022615  s8i          a9, a8, 0x46
42022618  call8        0x42038810
4202261b  l32r         a10, 0x41f40de4
4202261e  addi         a11, a1, 0x8
42022621  call8        0x4201a428
42022624  l32r         a10, 0x41f40dd8
42022627  movi.n       a11, 0x1
42022629  call8        0x42022fe4
4202262c  retw.n       
42022630  entry        a1, 0x20
42022633  l32r         a10, 0x41f40dd8
```


## Target `FUN_42038cac` at `42038cac`

Reference count: 5

- `UNCONDITIONAL_CALL` from `4201a646` in `FUN_4201a5e8 @ 4201a5e8`

```asm
4201a62d  call8        0x42039004
4201a630  retw.n       
4201a634  l16si        a15, a4, 0xa
4201a637  mov.n        a10, a5
4201a639  s32i.n       a2, a1, 0x8
4201a63b  s32i.n       a2, a1, 0x4
4201a63d  s32i.n       a8, a1, 0x0
4201a63f  movi         a14, 0xf0
4201a642  mov.n        a13, a2
4201a644  mov.n        a12, a2
4201a646  call8        0x42038cac
4201a649  mov.n        a11, a2
4201a64b  mov.n        a10, a5
4201a64d  call8        0x42038fe4
4201a650  j            0x4201a630
4201a655  l16ui        a13, a3, 0x46
4201a658  l16ui        a15, a3, 0x4a
4201a65b  sext         a5, a13, 0xf
4201a65e  mul16u       a13, a13, a15
4201a661  extui        a13, a13, 0x0, 0x10
```

### Caller decompile `FUN_4201a5e8` `4201a5e8`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char FUN_4201a5e8(int *param_1)

{
  short sVar1;
  int iVar2;
  char cVar3;
  int *piVar4;
  ushort uVar5;
  uint uVar6;
  int unaff_a12;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int unaff_a14;
  uint uVar10;
  
  cVar3 = (char)param_1[0xc4f];
  if (*(ushort *)(param_1 + 0xc52) == 1) {
    piVar4 = param_1 + 9;
    if (cVar3 == '\0') {
      sVar1 = *(short *)((int)param_1 + 10);
      FUN_42038cac(piVar4,param_1 + 0x1f,0,0,0xf0,(int)sVar1,(char)param_1[0xc50],0,0);
      FUN_42038fe4(piVar4,0,0,0,0xf0,(int)sVar1);
    }
    else {
      cVar3 = '\0';
      sVar1 = *(short *)((int)param_1 + 0x3152);
      FUN_42038e2c(piVar4,param_1 + 0x1f,(int)(short)param_1[0xc53],
                   (int)*(short *)((int)param_1 + 0x314e),(int)(short)param_1[0xc54],(int)sVar1,
                   (char)param_1[0xc50],0,0);
      FUN_42039004(piVar4,(int)(short)param_1[0xc53],(int)*(short *)((int)param_1 + 0x314e),
                   (int)(short)param_1[0xc54],(int)*(short *)((int)param_1 + 0x3152),(int)sVar1);
    }
  }
  else {
    uVar10 = (uint)*(ushort *)((int)param_1 + 0x314a);
    uVar7 = *(ushort *)((int)param_1 + 0x3146) * uVar10;
    uVar8 = uVar7 & 0xffff;
    if (cVar3 == '\0') {
      if (*(char *)((int)param_1 + 0x313d) == '\0') {
        uVar8 = *(ushort *)((int)param_1 + 10) - uVar8;
        uVar5 = (ushort)uVar8;
        if (uVar10 <= (uVar8 & 0xffff)) {
          uVar5 = *(ushort *)((int)param_1 + 0x314a);
        }
        uVar10 = (uint)(short)uVar5;
        unaff_a14 = 0xf0;
        uVar8 = (int)(uVar7 * 0x10000) >> 0x10;
        FUN_42038cac(param_1 + 9,param_1 + 0x1f,0,uVar8,0xf0,uVar10,0,0,0);
        unaff_a12 = 0;
      }
      iVar2 = (*(ushort *)((int)param_1 + 0x3146) + 1) * 0x10000;
      *(short *)((int)param_1 + 0x3146) = (short)((uint)iVar2 >> 0x10);
      if ((int)(short)param_1[0xc52] == iVar2 >> 0x10) {
        *(undefined2 *)((int)param_1 + 0x3146) = 0;
        if (*(char *)((int)param_1 + 0x313d) != '\0') {
          FUN_42038f48(param_1 + 9,*(char *)((int)param_1 + 0x313d),unaff_a12,uVar8,unaff_a14,uVar10
                      );
          return '\0';
        }
        FUN_42038fe4(param_1 + 9,0,unaff_a12,uVar8,unaff_a14,uVar10);
        *(undefined1 *)((int)param_1 + 0x313d) = 1;
      }
      (**(code **)(*param_1 + 0x4c))(param_1,_DAT_41f402d4,unaff_a12,uVar8,unaff_a14,uVar10);
      cVar3 = '\x01';
    }
    else {
      if ((int)(short)*(ushort *)((int)param_1 + 0x3146) <
          (int)((*(ushort *)(param_1 + 0xc52) - 1) * 0x10000) >> 0x10) {
        uVar7 = uVar10 + uVar8 & 0xffff;
      }
      else {
        uVar7 = (uint)*(ushort *)((int)param_1 + 10);
      }
      uVar6 = (uint)*(ushort *)((int)param_1 + 0x314e);
      uVar10 = uVar6 + *(ushort *)((int)param_1 + 0x3152) & 0xffff;
      uVar7 = uVar6 + uVar7 & 0xffff;
      uVar9 = uVar6 + uVar8 & 0xffff;
      if (uVar7 <= uVar10) {
        uVar10 = uVar7;
      }
      if ((uVar9 < uVar10) && (*(char *)((int)param_1 + 0x313d) == '\0')) {
        unaff_a14 = (int)(short)param_1[0xc54];
        unaff_a12 = (int)(short)param_1[0xc53];
        uVar10 = (int)((uVar10 - uVar9) * 0x10000) >> 0x10;
        uVar9 = (int)((uVar6 + uVar8) * 0x10000) >> 0x10;
        FUN_42038e2c(param_1 + 9,param_1 + 0x1f,unaff_a12,uVar9,unaff_a14,uVar10,0,0,0);
      }
      iVar2 = (*(ushort *)((int)param_1 + 0x3146) + 1) * 0x10000;
      *(short *)((int)param_1 + 0x3146) = (short)((uint)iVar2 >> 0x10);
      if ((int)(short)param_1[0xc52] == iVar2 >> 0x10) {
        *(undefined2 *)((int)param_1 + 0x3146) = 0;
        if (*(char *)((int)param_1 + 0x313d) != '\0') {
          return '\0';
        }
        unaff_a14 = (int)*(short *)((int)param_1 + 0x3152);
        uVar9 = (uint)(short)param_1[0xc54];
        unaff_a12 = (int)*(short *)((int)param_1 + 0x314e);
        FUN_42039004(param_1 + 9,(int)(short)param_1[0xc53],unaff_a12,uVar9,unaff_a14,uVar10);
        *(undefined1 *)((int)param_1 + 0x313d) = 1;
      }
      (**(code **)(*param_1 + 0x4c))(param_1,_DAT_41f402d4,unaff_a12,uVar9,unaff_a14,uVar10);
    }
  }
  return cVar3;
}


```

- `UNCONDITIONAL_CALL` from `4201a729` in `FUN_4201a5e8 @ 4201a5e8`

```asm
4201a70d  extui        a8, a8, 0x0, 0x10
4201a710  minu         a15, a8, a15
4201a713  s32i.n       a2, a1, 0x4
4201a715  s32i.n       a2, a1, 0x0
4201a717  sext         a15, a15, 0xf
4201a71a  movi         a14, 0xf0
4201a71d  sext         a13, a13, 0xf
4201a720  mov          a12, a2
4201a723  addi         a11, a4, 0x7c
4201a726  addi         a10, a4, 0x24
4201a729  call8        0x42038cac
4201a72c  l16ui        a8, a3, 0x46
4201a72f  l16si        a5, a3, 0x48
4201a732  addi.n       a8, a8, 0x1
4201a734  sext         a8, a8, 0xf
4201a737  s16i         a8, a3, 0x46
4201a73a  bne          a5, a8, 0x4201a754
4201a73d  movi.n       a5, 0x0
4201a73f  l8ui         a11, a3, 0x3d
4201a742  s16i         a5, a3, 0x46
```

- `UNCONDITIONAL_CALL` from `4201a55a` in `FUN_4201a510 @ 4201a510`

```asm
4201a541  mov.n        a10, a4
4201a543  call8        0x42038f48
4201a546  retw.n       
4201a548  s32i         a3, a1, 0x8
4201a54b  s32i         a3, a1, 0x4
4201a54e  s32i         a3, a1, 0x0
4201a551  movi         a14, 0xf0
4201a554  mov.n        a13, a3
4201a556  mov.n        a12, a3
4201a558  mov.n        a10, a4
4201a55a  call8        0x42038cac
4201a55d  j            0x4201a538
4201a560  entry        a1, 0x20
4201a563  l32r         a3, 0x41f40de4
4201a566  movi         a11, 0xff
4201a569  mov          a10, a3
4201a56c  call8        0x4201866c
4201a56f  mov          a10, a3
4201a572  movi         a11, 0x1
4201a575  call8        0x4201a510
```

### Caller decompile `FUN_4201a510` `4201a510`

```c

int FUN_4201a510(int param_1,char param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1 + 0x24;
  iVar2 = (int)*(short *)((int)&DAT_00003148 + param_1 + 2);
  if (param_2 == '\0') {
    FUN_42038cac(iVar1,param_1 + 0x7c,0,0,0xf0,iVar2,0,0,0);
  }
  else {
    FUN_42038e2c(iVar1,param_1 + 0x7c,0,0,0xf0,iVar2,0,0,0);
  }
  FUN_42038fe4(iVar1,param_2,0,0,0xf0,iVar2);
  if (param_2 == '\0') {
    FUN_42038f48(iVar1,0,0,0,0xf0,iVar2);
  }
  return param_1 + 0x3100;
}


```

- `DATA` from `000a6cf4` in `<none>`

```asm
0008944f  l32i.n       a9, a9, 0x10
0008e0b8  beqi         a5, 0xa, 0x0008e0fd
0008e0d5  bbci         a4, 0xa, 0x0008e142
0008e0d9  add.n        a8, a6, a5
0008e0db  s32i.n       a2, a9, 0x20
0008e0dd  l32i         a9, a8, 0x64
0008e0f3  l32i.n       a9, a4, 0x30
0008e0f5  l32i.n       a2, a13, 0x34
000a5455  call12       0x0008944c
000a5458  call12       0x0010a740
000a545b  extui        a3, a11, 0x16, 0xf
000a7025  ill          
000a7028  ill          
000a702b  ill          
000a702e  ill          
000a7031  ill          
000a7034  ill          
000a7037  ill          
000a703a  ill          
000a703d  ill          
```

- `DATA` from `3c236cf4` in `<none>`

```asm
002bca4f  movi.n       a4, 0x0
002bca51  xsr          a4, INTENABLE
002bca54  rsync        
002bca57  or           a3, a4, a2
002bca5a  xor          a3, a3, a2
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
4037400c  rfwo         
40375660  entry        a1, 0x20
40375663  l32r         a8, 0x402b4408
40375666  l32i.n       a8, a8, 0x0
40375668  beqz.n       a8, 0x40375678
```


## Target `FUN_4203885c` at `4203885c`

Reference count: 2

- `UNCONDITIONAL_CALL` from `42038d0d` in `FUN_42038cac @ 42038cac`

```asm
42038cf8  l32i.n       a8, a1, 0x14
42038cfa  mov.n        a15, a6
42038cfc  s32i.n       a8, a1, 0x4
42038cfe  mov.n        a14, a5
42038d00  mov.n        a13, a4
42038d02  mov.n        a12, a3
42038d04  movi.n       a11, 0x13
42038d06  call8        0x4203889c
42038d09  mov.n        a11, a3
42038d0b  mov.n        a10, a2
42038d0d  call8        0x4203885c
42038d10  retw.n       
42038d14  entry        a1, 0x20
42038d17  call8        0x420673a8
42038e2c  entry        a1, 0x50
42038e2f  s32i.n       a3, a1, 0x1c
42038e31  sext         a3, a5, 0xf
42038e34  l8ui         a5, a1, 0x50
42038e37  l8ui         a8, a1, 0x54
42038e3a  s32i.n       a5, a1, 0x20
```

### Caller decompile `FUN_42038cac` `42038cac`

```c

undefined4
FUN_42038cac(undefined4 param_1,undefined4 param_2,short param_3,short param_4,short param_5,
            short param_6,undefined1 param_7,undefined1 param_8,undefined1 param_9)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = (int)param_3;
  iVar2 = (int)param_4;
  FUN_42038b60(param_1);
  iVar3 = (int)param_5;
  FUN_4203889c(param_1,0x10,param_2,iVar1,iVar2,iVar3,(int)param_6,param_7,param_8,param_9);
  FUN_4203889c(param_1,0x13,param_2,iVar1,iVar2,iVar3,(int)param_6,param_7,param_8,param_9);
  FUN_4203885c(param_1,param_2,param_2,iVar1,iVar2,iVar3);
  return param_1;
}


```

- `UNCONDITIONAL_CALL` from `42038887` in `FUN_42038870 @ 42038870`

```asm
4203886d  retw.n       
42038870  entry        a1, 0x20
42038873  l32r         a11, 0x41f42284
42038876  mov          a10, a3
42038879  call8        0x420042e4
4203887c  mov          a4, a2
4203887f  mov.n        a2, a10
42038881  beqz.n       a10, 0x4203888a
42038883  mov.n        a11, a3
42038885  mov.n        a10, a4
42038887  call8        0x4203885c
4203888a  retw.n       
4203888c  entry        a1, 0x20
4203888f  l32r         a11, 0x41f42284
42038892  mov.n        a10, a3
42038894  call8        0x420042b8
42038897  retw.n       
4203889c  entry        a1, 0x20
4203889f  extui        a11, a3, 0x0, 0x8
420388a2  mov.n        a10, a2
```

### Caller decompile `FUN_42038870` `42038870`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_42038870(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = FUN_420042e4(param_2,_DAT_41f42284);
  if (iVar1 != 0) {
    FUN_4203885c(param_1,param_2);
  }
  return iVar1;
}


```


## Target `FUN_4203889c` at `4203889c`

Reference count: 2

- `UNCONDITIONAL_CALL` from `42038ce9` in `FUN_42038cac @ 42038cac`

```asm
42038cd5  s32i.n       a8, a1, 0x8
42038cd7  l32i.n       a8, a1, 0x14
42038cd9  mov.n        a15, a6
42038cdb  s32i.n       a8, a1, 0x4
42038cdd  mov.n        a14, a5
42038cdf  mov.n        a13, a4
42038ce1  mov.n        a12, a3
42038ce3  mov.n        a10, a2
42038ce5  s32i.n       a7, a1, 0x0
42038ce7  movi.n       a11, 0x10
42038ce9  call8        0x4203889c
42038cec  l32i.n       a8, a1, 0x1c
42038cee  mov.n        a10, a2
42038cf0  s32i.n       a8, a1, 0xc
42038cf2  l32i.n       a8, a1, 0x18
42038cf4  s32i.n       a7, a1, 0x0
42038cf6  s32i.n       a8, a1, 0x8
42038cf8  l32i.n       a8, a1, 0x14
42038cfa  mov.n        a15, a6
42038cfc  s32i.n       a8, a1, 0x4
```

- `UNCONDITIONAL_CALL` from `42038d06` in `FUN_42038cac @ 42038cac`

```asm
42038cf2  l32i.n       a8, a1, 0x18
42038cf4  s32i.n       a7, a1, 0x0
42038cf6  s32i.n       a8, a1, 0x8
42038cf8  l32i.n       a8, a1, 0x14
42038cfa  mov.n        a15, a6
42038cfc  s32i.n       a8, a1, 0x4
42038cfe  mov.n        a14, a5
42038d00  mov.n        a13, a4
42038d02  mov.n        a12, a3
42038d04  movi.n       a11, 0x13
42038d06  call8        0x4203889c
42038d09  mov.n        a11, a3
42038d0b  mov.n        a10, a2
42038d0d  call8        0x4203885c
42038d10  retw.n       
42038d14  entry        a1, 0x20
42038d17  call8        0x420673a8
42038e2c  entry        a1, 0x50
42038e2f  s32i.n       a3, a1, 0x1c
42038e31  sext         a3, a5, 0xf
```


## `42038c54`

No containing function.

## Target `FUN_42038fe4` at `42038fe4`

Reference count: 5

- `UNCONDITIONAL_CALL` from `4201a64d` in `FUN_4201a5e8 @ 4201a5e8`

```asm
4201a637  mov.n        a10, a5
4201a639  s32i.n       a2, a1, 0x8
4201a63b  s32i.n       a2, a1, 0x4
4201a63d  s32i.n       a8, a1, 0x0
4201a63f  movi         a14, 0xf0
4201a642  mov.n        a13, a2
4201a644  mov.n        a12, a2
4201a646  call8        0x42038cac
4201a649  mov.n        a11, a2
4201a64b  mov.n        a10, a5
4201a64d  call8        0x42038fe4
4201a650  j            0x4201a630
4201a655  l16ui        a13, a3, 0x46
4201a658  l16ui        a15, a3, 0x4a
4201a65b  sext         a5, a13, 0xf
4201a65e  mul16u       a13, a13, a15
4201a661  extui        a13, a13, 0x0, 0x10
4201a664  beqz         a2, 0x4201a700
4201a667  addi.n       a8, a8, -0x1
4201a669  sext         a8, a8, 0xf
```

- `UNCONDITIONAL_CALL` from `4201a74e` in `FUN_4201a5e8 @ 4201a5e8`

```asm
4201a732  addi.n       a8, a8, 0x1
4201a734  sext         a8, a8, 0xf
4201a737  s16i         a8, a3, 0x46
4201a73a  bne          a5, a8, 0x4201a754
4201a73d  movi.n       a5, 0x0
4201a73f  l8ui         a11, a3, 0x3d
4201a742  s16i         a5, a3, 0x46
4201a745  addi         a10, a4, 0x24
4201a748  bnez         a11, 0x4201a768
4201a74b  movi         a2, 0x1
4201a74e  call8        0x42038fe4
4201a751  s8i          a2, a3, 0x3d
4201a754  l32i.n       a2, a4, 0x0
4201a756  l32r         a11, 0x41f402d4
4201a759  l32i         a2, a2, 0x4c
4201a75c  mov.n        a10, a4
4201a75e  callx8       a2
4201a761  movi.n       a2, 0x1
4201a763  j            0x4201a630
4201a768  call8        0x42038f48
```

- `UNCONDITIONAL_CALL` from `4201a53c` in `FUN_4201a510 @ 4201a510`

```asm
4201a525  movi.n       a12, 0x0
4201a527  s32i.n       a12, a1, 0x8
4201a529  s32i.n       a12, a1, 0x4
4201a52b  s32i.n       a12, a1, 0x0
4201a52d  movi         a14, 0xf0
4201a530  mov.n        a13, a12
4201a532  mov          a10, a4
4201a535  call8        0x42038e2c
4201a538  mov.n        a11, a3
4201a53a  mov.n        a10, a4
4201a53c  call8        0x42038fe4
4201a53f  bnez.n       a3, 0x4201a546
4201a541  mov.n        a10, a4
4201a543  call8        0x42038f48
4201a546  retw.n       
4201a548  s32i         a3, a1, 0x8
4201a54b  s32i         a3, a1, 0x4
4201a54e  s32i         a3, a1, 0x0
4201a551  movi         a14, 0xf0
4201a554  mov.n        a13, a3
```

- `DATA` from `000a6d10` in `<none>`

```asm
0008944f  l32i.n       a9, a9, 0x10
0008e0b8  beqi         a5, 0xa, 0x0008e0fd
0008e0d5  bbci         a4, 0xa, 0x0008e142
0008e0d9  add.n        a8, a6, a5
0008e0db  s32i.n       a2, a9, 0x20
0008e0dd  l32i         a9, a8, 0x64
0008e0f3  l32i.n       a9, a4, 0x30
0008e0f5  l32i.n       a2, a13, 0x34
000a5455  call12       0x0008944c
000a5458  call12       0x0010a740
000a545b  extui        a3, a11, 0x16, 0xf
000a7025  ill          
000a7028  ill          
000a702b  ill          
000a702e  ill          
000a7031  ill          
000a7034  ill          
000a7037  ill          
000a703a  ill          
000a703d  ill          
```

- `DATA` from `3c236d10` in `<none>`

```asm
002bca4f  movi.n       a4, 0x0
002bca51  xsr          a4, INTENABLE
002bca54  rsync        
002bca57  or           a3, a4, a2
002bca5a  xor          a3, a3, a2
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
4037400c  rfwo         
40375660  entry        a1, 0x20
40375663  l32r         a8, 0x402b4408
40375666  l32i.n       a8, a8, 0x0
40375668  beqz.n       a8, 0x40375678
```


## Target `FUN_42039004` at `42039004`

Reference count: 4

- `UNCONDITIONAL_CALL` from `4201a62d` in `FUN_4201a5e8 @ 4201a5e8`

```asm
4201a611  l16si        a13, a3, 0x4e
4201a614  l16si        a12, a3, 0x4c
4201a617  l16si        a15, a3, 0x52
4201a61a  mov.n        a10, a5
4201a61c  call8        0x42038e2c
4201a61f  l16si        a14, a3, 0x52
4201a622  l16si        a13, a3, 0x50
4201a625  l16si        a12, a3, 0x4e
4201a628  l16si        a11, a3, 0x4c
4201a62b  mov.n        a10, a5
4201a62d  call8        0x42039004
4201a630  retw.n       
4201a634  l16si        a15, a4, 0xa
4201a637  mov.n        a10, a5
4201a639  s32i.n       a2, a1, 0x8
4201a63b  s32i.n       a2, a1, 0x4
4201a63d  s32i.n       a8, a1, 0x0
4201a63f  movi         a14, 0xf0
4201a642  mov.n        a13, a2
4201a644  mov.n        a12, a2
```

- `UNCONDITIONAL_CALL` from `4201a6e2` in `FUN_4201a5e8 @ 4201a5e8`

```asm
4201a6c6  movi.n       a5, 0x0
4201a6c8  s16i         a5, a3, 0x46
4201a6cb  l8ui         a5, a3, 0x3d
4201a6ce  bnez         a5, 0x4201a771
4201a6d1  l16si        a14, a3, 0x52
4201a6d4  l16si        a13, a3, 0x50
4201a6d7  l16si        a12, a3, 0x4e
4201a6da  l16si        a11, a3, 0x4c
4201a6dd  addi         a10, a4, 0x24
4201a6e0  movi.n       a5, 0x1
4201a6e2  call8        0x42039004
4201a6e5  s8i          a5, a3, 0x3d
4201a6e8  l32i.n       a3, a4, 0x0
4201a6ea  l32r         a11, 0x41f402d4
4201a6ed  l32i         a3, a3, 0x4c
4201a6f0  mov.n        a10, a4
4201a6f2  callx8       a3
4201a6f5  j            0x4201a630
4201a6f8  l16ui        a9, a4, 0xa
4201a6fb  j            0x4201a677
```

- `DATA` from `000a6d14` in `<none>`

```asm
0008944f  l32i.n       a9, a9, 0x10
0008e0b8  beqi         a5, 0xa, 0x0008e0fd
0008e0d5  bbci         a4, 0xa, 0x0008e142
0008e0d9  add.n        a8, a6, a5
0008e0db  s32i.n       a2, a9, 0x20
0008e0dd  l32i         a9, a8, 0x64
0008e0f3  l32i.n       a9, a4, 0x30
0008e0f5  l32i.n       a2, a13, 0x34
000a5455  call12       0x0008944c
000a5458  call12       0x0010a740
000a545b  extui        a3, a11, 0x16, 0xf
000a7025  ill          
000a7028  ill          
000a702b  ill          
000a702e  ill          
000a7031  ill          
000a7034  ill          
000a7037  ill          
000a703a  ill          
000a703d  ill          
```

- `DATA` from `3c236d14` in `<none>`

```asm
002bca4f  movi.n       a4, 0x0
002bca51  xsr          a4, INTENABLE
002bca54  rsync        
002bca57  or           a3, a4, a2
002bca5a  xor          a3, a3, a2
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
4037400c  rfwo         
40375660  entry        a1, 0x20
40375663  l32r         a8, 0x402b4408
40375666  l32i.n       a8, a8, 0x0
40375668  beqz.n       a8, 0x40375678
```

