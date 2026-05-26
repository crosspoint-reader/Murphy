# Function Reference Report

## Target `FUN_42038280` at `42038280`

Reference count: 1

- `UNCONDITIONAL_CALL` from `42038844` in `FUN_42038810 @ 42038810`

```asm
4203882c  s32i.n       a8, a1, 0x14
4203882e  s32i.n       a9, a1, 0x0
42038830  s32i.n       a7, a1, 0x10
42038832  mov.n        a15, a7
42038834  sext         a14, a6, 0xf
42038837  sext         a13, a5, 0xf
4203883a  sext         a12, a4, 0xf
4203883d  sext         a11, a3, 0xf
42038840  mov.n        a10, a2
42038842  s32i.n       a8, a1, 0x2c
42038844  call8        0x42038280
42038847  l32r         a9, 0x41f42280
4203884a  l32i.n       a8, a1, 0x2c
4203884c  s32i.n       a9, a2, 0x0
4203884e  s16i         a8, a2, 0x40
42038851  s32i         a7, a2, 0x50
42038854  s32i         a7, a2, 0x54
42038857  call8        0x420387dc
4203885a  retw.n       
4203885c  entry        a1, 0x20
```

### Caller decompile `FUN_42038810` `42038810`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 *
FUN_42038810(undefined4 *param_1,short param_2,short param_3,short param_4,short param_5)

{
  undefined4 uVar1;
  
  uVar1 = FUN_42038280(param_1,(int)param_2,(int)param_3,(int)param_4,(int)param_5,0,_DAT_41f4227c,
                       0xf0,0x1a0,0x21,0,1,1);
  *param_1 = _DAT_41f42280;
  *(undefined2 *)(param_1 + 0x10) = 1;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  FUN_420387dc(uVar1,(int)param_2,(int)param_3,(int)param_4,(int)param_5,0);
  return param_1;
}


```


## Target `FUN_420389ec` at `420389ec`

Reference count: 1

- `UNCONDITIONAL_CALL` from `42038b67` in `FUN_42038b60 @ 42038b60`

```asm
42038b4c  mov.n        a10, a2
42038b4e  movi         a11, 0x50
42038b51  call8        0x4203844c
42038b54  movi         a11, 0x97
42038b57  mov.n        a10, a2
42038b59  call8        0x420384ac
42038b5c  j            0x42038aa6
42038b60  entry        a1, 0x20
42038b63  movi.n       a11, 0x0
42038b65  mov.n        a10, a2
42038b67  call8        0x420389ec
42038b6a  l32r         a3, 0x41f4226c
42038b6d  movi.n       a11, 0x20
42038b6f  l8ui         a3, a3, 0x0
42038b72  mov.n        a10, a2
42038b74  beqz         a3, 0x42038c05
42038b77  call8        0x4203844c
42038b7a  l32r         a11, 0x41f422a4
42038b7d  movi.n       a12, 0x38
42038b7f  mov.n        a10, a2
```

### Caller decompile `FUN_42038b60` `42038b60`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_42038b60(int param_1)

{
  char *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  FUN_420389ec(param_1,0);
  if (*_DAT_41f4226c == '\0') {
    FUN_4203844c(param_1,0x20);
    FUN_420385ac(param_1,_DAT_41f422bc,0x2a);
    FUN_4203844c(param_1,0x21,0x2a);
    FUN_420385ac(param_1,_DAT_41f422c0,0x2a);
    FUN_4203844c(param_1,0x22,0x2a);
    FUN_420385ac(param_1,_DAT_41f422c4,0x2a);
    FUN_4203844c(param_1,0x23,0x2a);
    FUN_420385ac(param_1,_DAT_41f422c8,0x2a);
    FUN_4203844c(param_1,0x24,0x2a);
    uVar3 = _DAT_41f422cc;
  }
  else {
    FUN_4203844c(param_1,0x20);
    FUN_420385ac(param_1,_DAT_41f422a4,0x38);
    FUN_4203844c(param_1,0x21,0x38);
    FUN_420385ac(param_1,_DAT_41f422a8,0x2a);
    FUN_4203844c(param_1,0x24,0x2a);
    FUN_420385ac(param_1,_DAT_41f422ac,0x2a);
    uVar3 = _DAT_41f422b8;
    uVar2 = _DAT_41f422b4;
    pcVar1 = _DAT_41f422b0;
    if (*_DAT_41f422b0 != '\0') {
      FUN_4203844c(param_1,0x22,0x2a);
      FUN_420385ac(param_1,uVar3,0x2a);
      FUN_4203844c(param_1,0x23,0x2a);
      FUN_420385ac(param_1,uVar2,0x38);
      *pcVar1 = '\0';
      goto LAB_42038bfb;
    }
    FUN_4203844c(param_1,0x22,0x2a);
    FUN_420385ac(param_1,uVar2,0x38);
    FUN_4203844c(param_1,0x23,0x38);
  }
  FUN_420385ac(param_1,uVar3,0x2a);
LAB_42038bfb:
  *(undefined1 *)(param_1 + 0x33) = 0;
  return param_1;
}


```


## Target `FUN_420389cc` at `420389cc`

Reference count: 1

- `UNCONDITIONAL_CALL` from `42038af7` in `FUN_420389ec @ 420389ec`

```asm
42038ade  movi         a11, 0xd7
42038ae1  mov.n        a10, a2
42038ae3  call8        0x420384ac
42038ae6  movi         a11, 0xd7
42038ae9  mov.n        a10, a2
42038aeb  call8        0x420384ac
42038aee  movi.n       a11, 0x1f
42038af0  mov.n        a10, a2
42038af2  call8        0x420384ac
42038af5  mov.n        a10, a2
42038af7  call8        0x420389cc
42038afa  movi.n       a11, 0x0
42038afc  mov.n        a10, a2
42038afe  call8        0x4203844c
42038b01  movi         a11, 0xff
42038b04  mov.n        a10, a2
42038b06  call8        0x420384ac
42038b09  movi.n       a11, 0x30
42038b0b  mov.n        a10, a2
42038b0d  call8        0x4203844c
```

### Caller decompile `FUN_420389ec` `420389ec`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_420389ec(int param_1,char param_2)

{
  undefined1 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 uStack_2d;
  undefined1 uStack_2c;
  undefined1 uStack_2b;
  undefined1 uStack_2a;
  undefined1 uStack_29;
  undefined1 uStack_28;
  undefined1 uStack_27;
  undefined1 uStack_26;
  undefined1 auStack_25 [37];
  
  FUN_420382fc(param_1);
  while (iVar3 = FUN_42066740(*(undefined1 *)(param_1 + 0x16)), iVar3 == 0) {
    FUN_420673d4(10);
  }
  if (*_DAT_41f4226c == '\0') {
    FUN_4203844c(param_1,1);
    FUN_420384ac(param_1,3);
    FUN_420384ac(param_1,0x10);
    FUN_420384ac(param_1,0x3f);
    FUN_420384ac(param_1,0x3b);
    uVar1 = 0;
    if (param_2 == '\0') {
      uVar1 = 0xd;
    }
    FUN_420384ac(param_1,uVar1);
    FUN_4203844c(param_1,6);
    FUN_420384ac(param_1,0xd7);
    FUN_420384ac(param_1,0xd7);
    FUN_420384ac(param_1,0x1f);
    FUN_420389cc(param_1,0x1f);
    FUN_4203844c(param_1,0);
    FUN_420384ac(param_1,0xff);
    FUN_4203844c(param_1,0x30);
    FUN_420384ac(param_1,9);
    FUN_4203844c(param_1,0x61);
    FUN_420384ac(param_1,0xf0);
    FUN_420384ac(param_1,1);
    FUN_420384ac(param_1,0xa0);
    FUN_4203844c(param_1,0x82);
    uVar2 = 0;
    if (param_2 == '\0') {
      uVar2 = 0xf;
    }
    FUN_420384ac(param_1,uVar2);
    FUN_4203844c(param_1,0x50);
    FUN_420384ac(param_1,0x97);
  }
  else {
    uStack_2d = *_DAT_41f42294;
    uStack_2c = _DAT_41f42294[1];
    FUN_42038544(param_1,0,&uStack_2d,2);
    (*_DAT_41f41eb0)(auStack_25,_DAT_41f42298,5,2);
    FUN_42038544(param_1,1,auStack_25,5);
    uStack_2b = *_DAT_41f4229c;
    uStack_2a = _DAT_41f4229c[1];
    uStack_29 = _DAT_41f4229c[2];
    FUN_42038544(param_1,6,&uStack_2b,3);
    FUN_420384e8(param_1,0x30,9,3);
    FUN_420384e8(param_1,0x82,7,3);
    uStack_28 = *_DAT_41f422a0;
    uStack_27 = _DAT_41f422a0[1];
    uStack_26 = _DAT_41f422a0[2];
    FUN_42038544(param_1,0x61,&uStack_28,3);
    FUN_420384e8(param_1,0x50,0xd7,3);
  }
  return param_1;
}


```


## Target `FUN_42038f24` at `42038f24`

Reference count: 2

- `UNCONDITIONAL_CALL` from `42038f4d` in `FUN_42038f48 @ 42038f48`

```asm
42038f30  call8        0x4203844c
42038f33  l32r         a11, 0x41f422fc
42038f36  movi         a12, 0x64
42038f39  mov.n        a10, a2
42038f3b  call8        0x42038970
42038f3e  movi.n       a8, 0x0
42038f40  s16i         a8, a2, 0x32
42038f43  retw.n       
42038f48  entry        a1, 0x20
42038f4b  mov.n        a10, a2
42038f4d  call8        0x42038f24
42038f50  retw.n       
42038f54  entry        a1, 0x20
42038f57  mov.n        a10, a2
42038f59  call8        0x42038f24
42038f5c  movi.n       a11, 0x7
42038f5e  mov.n        a10, a2
42038f60  call8        0x4203844c
42038f63  movi         a11, 0xa5
42038f66  mov          a10, a2
```

### Caller decompile `FUN_42038f48` `42038f48`

```c

undefined4 FUN_42038f48(undefined4 param_1)

{
  FUN_42038f24(param_1);
  return param_1;
}


```

- `UNCONDITIONAL_CALL` from `42038f59` in `FUN_42038f54 @ 42038f54`

```asm
42038f3b  call8        0x42038970
42038f3e  movi.n       a8, 0x0
42038f40  s16i         a8, a2, 0x32
42038f43  retw.n       
42038f48  entry        a1, 0x20
42038f4b  mov.n        a10, a2
42038f4d  call8        0x42038f24
42038f50  retw.n       
42038f54  entry        a1, 0x20
42038f57  mov.n        a10, a2
42038f59  call8        0x42038f24
42038f5c  movi.n       a11, 0x7
42038f5e  mov.n        a10, a2
42038f60  call8        0x4203844c
42038f63  movi         a11, 0xa5
42038f66  mov          a10, a2
42038f69  call8        0x420384ac
42038f6c  movi.n       a8, 0x1
42038f6e  s8i          a8, a2, 0x34
42038f71  retw.n       
```

### Caller decompile `FUN_42038f54` `42038f54`

```c

int FUN_42038f54(int param_1)

{
  FUN_42038f24(param_1);
  FUN_4203844c(param_1,7);
  FUN_420384ac(param_1,0xa5);
  *(undefined1 *)(param_1 + 0x34) = 1;
  return param_1;
}


```


## Target `FUN_42038f74` at `42038f74`

Reference count: 1

- `UNCONDITIONAL_CALL` from `42038ff8` in `FUN_42038fe4 @ 42038fe4`

```asm
42038fda  movi.n       a11, 0x12
42038fdc  mov.n        a10, a2
42038fde  call8        0x4203844c
42038fe1  j            0x42038fbc
42038fe4  entry        a1, 0x20
42038fe7  extui        a3, a3, 0x0, 0x8
42038fea  mov          a10, a2
42038fed  beqz         a3, 0x42038ff8
42038ff0  call8        0x42038fa4
42038ff3  retw         
42038ff8  call8        0x42038f74
42038ffb  s8i          a3, a2, 0x31
42038ffe  j            0x42038ff3
42039004  entry        a1, 0x20
42039007  mov.n        a10, a2
42039009  call8        0x42038fa4
4203900c  retw.n       
42039010  entry        a1, 0x20
42039013  l32r         a11, 0x41f42308
42039016  l32r         a10, 0x41f42264
```

### Caller decompile `FUN_42038fe4` `42038fe4`

```c

int FUN_42038fe4(int param_1,char param_2)

{
  if (param_2 == '\0') {
    FUN_42038f74(param_1);
    *(undefined1 *)(param_1 + 0x31) = 0;
  }
  else {
    FUN_42038fa4(param_1);
  }
  return param_1;
}


```


## Target `FUN_42038fa4` at `42038fa4`

Reference count: 2

- `UNCONDITIONAL_CALL` from `42038ff0` in `FUN_42038fe4 @ 42038fe4`

```asm
42038fd5  s8i          a8, a2, 0x40
42038fd8  retw.n       
42038fda  movi.n       a11, 0x12
42038fdc  mov.n        a10, a2
42038fde  call8        0x4203844c
42038fe1  j            0x42038fbc
42038fe4  entry        a1, 0x20
42038fe7  extui        a3, a3, 0x0, 0x8
42038fea  mov          a10, a2
42038fed  beqz         a3, 0x42038ff8
42038ff0  call8        0x42038fa4
42038ff3  retw         
42038ff8  call8        0x42038f74
42038ffb  s8i          a3, a2, 0x31
42038ffe  j            0x42038ff3
42039004  entry        a1, 0x20
42039007  mov.n        a10, a2
42039009  call8        0x42038fa4
4203900c  retw.n       
42039010  entry        a1, 0x20
```

- `UNCONDITIONAL_CALL` from `42039009` in `FUN_42039004 @ 42039004`

```asm
42038fe7  extui        a3, a3, 0x0, 0x8
42038fea  mov          a10, a2
42038fed  beqz         a3, 0x42038ff8
42038ff0  call8        0x42038fa4
42038ff3  retw         
42038ff8  call8        0x42038f74
42038ffb  s8i          a3, a2, 0x31
42038ffe  j            0x42038ff3
42039004  entry        a1, 0x20
42039007  mov.n        a10, a2
42039009  call8        0x42038fa4
4203900c  retw.n       
42039010  entry        a1, 0x20
42039013  l32r         a11, 0x41f42308
42039016  l32r         a10, 0x41f42264
42039019  movi.n       a2, 0x2
4203901b  call8        0x420641b8
4203901e  retw.n       
42039020  entry        a1, 0x20
42039023  l32i         a2, a2, 0x74
```

### Caller decompile `FUN_42039004` `42039004`

```c

undefined4 FUN_42039004(undefined4 param_1)

{
  FUN_42038fa4(param_1);
  return param_1;
}


```


## Target `FUN_42038b60` at `42038b60`

Reference count: 2

- `UNCONDITIONAL_CALL` from `42038cc6` in `FUN_42038cac @ 42038cac`

```asm
42038cac  entry        a1, 0x40
42038caf  l8ui         a8, a1, 0x40
42038cb2  mov.n        a10, a2
42038cb4  s32i.n       a8, a1, 0x14
42038cb6  l8ui         a8, a1, 0x44
42038cb9  sext         a4, a4, 0xf
42038cbc  s32i.n       a8, a1, 0x18
42038cbe  l8ui         a8, a1, 0x48
42038cc1  sext         a5, a5, 0xf
42038cc4  s32i.n       a8, a1, 0x1c
42038cc6  call8        0x42038b60
42038cc9  l32i.n       a8, a1, 0x1c
42038ccb  sext         a6, a6, 0xf
42038cce  s32i.n       a8, a1, 0xc
42038cd0  l32i.n       a8, a1, 0x18
42038cd2  sext         a7, a7, 0xf
42038cd5  s32i.n       a8, a1, 0x8
42038cd7  l32i.n       a8, a1, 0x14
42038cd9  mov.n        a15, a6
42038cdb  s32i.n       a8, a1, 0x4
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

- `UNCONDITIONAL_CALL` from `42038c5a` in `<none>`

```asm
42038c40  mov.n        a10, a2
42038c42  call8        0x420385ac
42038c45  movi.n       a11, 0x24
42038c47  mov.n        a10, a2
42038c49  call8        0x4203844c
42038c4c  movi.n       a12, 0x2a
42038c4e  l32r         a11, 0x41f422cc
42038c51  j            0x42038bcf
42038c54  entry        a1, 0x20
42038c57  mov          a10, a2
42038c5a  call8        0x42038b60
42038c5d  movi         a11, 0x10
42038c60  mov.n        a10, a2
42038c62  call8        0x4203844c
42038c65  mov.n        a10, a2
42038c67  call8        0x420385ec
42038c6a  l32r         a4, 0x41f42284
42038c6d  extui        a3, a3, 0x0, 0x8
42038c70  mov.n        a11, a3
42038c72  mov.n        a10, a2
```

