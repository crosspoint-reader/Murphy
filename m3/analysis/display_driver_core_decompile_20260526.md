# Decompile Report

## `0x42022618`

No function containing address.

## `0x42038280` in `FUN_42038280` at `42038280`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 *
FUN_42038280(undefined4 *param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4,
            undefined2 param_5,undefined2 param_6,undefined4 param_7,undefined2 param_8,
            undefined2 param_9,undefined4 param_10,undefined1 param_11,undefined1 param_12,
            undefined1 param_13)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  *(undefined1 *)((int)param_1 + 0xe) = param_13;
  param_1[7] = param_7;
  *(undefined1 *)(param_1 + 8) = 0;
  param_1[9] = _DAT_41f413ac;
  param_1[10] = _DAT_41f42258;
  *(undefined2 *)(param_1 + 0xb) = 1;
  param_1[0xc] = 0x101;
  uVar2 = _DAT_41f4225c;
  uVar1 = _DAT_41f42254;
  *(undefined2 *)((int)param_1 + 6) = param_9;
  param_1[0xd] = uVar2;
  *param_1 = uVar1;
  *(undefined2 *)(param_1 + 1) = param_8;
  param_1[2] = param_10;
  *(undefined1 *)(param_1 + 3) = param_11;
  *(undefined1 *)((int)param_1 + 0xd) = param_12;
  *(undefined2 *)(param_1 + 4) = param_2;
  *(undefined2 *)((int)param_1 + 0x12) = param_3;
  *(undefined2 *)(param_1 + 5) = param_4;
  *(undefined2 *)((int)param_1 + 0x16) = param_5;
  *(undefined2 *)(param_1 + 6) = param_6;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  return param_1;
}


```

## `0x420382fc` in `FUN_420382fc` at `420382fc`

```c

int FUN_420382fc(int param_1)

{
  ushort uVar1;
  
  if (-1 < (short)*(ushort *)(param_1 + 0x14)) {
    uVar1 = *(ushort *)(param_1 + 0x14) & 0xff;
    if (*(char *)(param_1 + 0x21) == '\0') {
      FUN_42066718(uVar1,1);
      FUN_4206661c(*(undefined1 *)(param_1 + 0x14),3);
      FUN_42066718(*(undefined1 *)(param_1 + 0x14),1);
      FUN_420673c4(2,1);
      FUN_42066718(*(undefined1 *)(param_1 + 0x14),0);
      FUN_420673c4(*(undefined2 *)(param_1 + 0x36),0);
      FUN_42066718(*(undefined1 *)(param_1 + 0x14),1);
    }
    else {
      FUN_42066718(uVar1,0);
      FUN_4206661c(*(undefined1 *)(param_1 + 0x14),3);
      FUN_42066718(*(undefined1 *)(param_1 + 0x14),0);
      FUN_420673c4(2,0);
      FUN_4206661c(*(undefined1 *)(param_1 + 0x14),5);
    }
    *(undefined1 *)(param_1 + 0x34) = 0;
  }
  return param_1;
}


```

## `0x42038648` in `FUN_42038648` at `42038648`

```c

char FUN_42038648(char param_1)

{
  char cVar1;
  
  cVar1 = '\b';
  do {
    FUN_42066718(4,0);
    FUN_42066718(3,param_1 < '\0');
    cVar1 = cVar1 + -1;
    param_1 = param_1 << 1;
    FUN_42066718(4,1);
  } while (cVar1 != '\0');
  FUN_42066718(4,0);
  return param_1;
}


```

## `0x4203868c` in `FUN_4203868c` at `4203868c`

```c

undefined1 FUN_4203868c(undefined1 param_1)

{
  FUN_42066718(6,0);
  FUN_42066718(5,0);
  FUN_42038648(param_1,0);
  FUN_42066718(5,1);
  FUN_42066718(6,1);
  return param_1;
}


```

## `0x420389cc` in `FUN_420389cc` at `420389cc`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_420389cc(int param_1)

{
  FUN_4203844c(param_1,4);
  FUN_42038970(param_1,_DAT_41f42290,100);
  *(undefined1 *)(param_1 + 0x32) = 1;
  return param_1;
}


```

## `0x420389ec` in `FUN_420389ec` at `420389ec`

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

## `0x42038b60` in `FUN_42038b60` at `42038b60`

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

## `0x42038f24` in `FUN_42038f24` at `42038f24`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_42038f24(int param_1)

{
  if (*(char *)(param_1 + 0x32) != '\0') {
    FUN_4203844c(param_1,2);
    FUN_42038970(param_1,_DAT_41f422fc,100);
  }
  *(undefined2 *)(param_1 + 0x32) = 0;
  return param_1;
}


```

## `0x42038f74` in `FUN_42038f74` at `42038f74`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_42038f74(undefined4 param_1)

{
  if (*_DAT_41f4226c == '\0') {
    FUN_4203844c(param_1,0x12);
  }
  else {
    FUN_420384e8(param_1,0x17,0xa5);
  }
  FUN_42038970(param_1,_DAT_41f42300,0x4e2);
  return param_1;
}


```

## `0x42038fa4` in `FUN_42038fa4` at `42038fa4`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_42038fa4(int param_1)

{
  if (*_DAT_41f4226c == '\0') {
    FUN_4203844c(param_1,0x12);
  }
  else {
    FUN_420384e8(param_1,0x17,0xa5);
  }
  FUN_42038970(param_1,_DAT_41f42304,600 / *(byte *)(param_1 + 0x40));
  if (1 < *(byte *)(param_1 + 0x40)) {
    *(undefined1 *)(param_1 + 0x40) = 1;
  }
  return param_1;
}


```

