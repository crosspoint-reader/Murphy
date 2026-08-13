# Decompile Report

## `0x4202e140` in `FUN_4202e140` at `4202e140`

```c

char FUN_4202e140(byte *param_1,undefined4 param_2)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  byte bVar4;

  bVar1 = *param_1;
  bVar4 = bVar1 & 0xf;
  if ((bVar1 < 0x10) && (bVar4 < 3)) {
    if ((bVar1 & 0xf) != 0) {
      iVar3 = FUN_4202e0b8(param_1 + 1,1,param_2);
      if (iVar3 == 0) goto LAB_4202e184;
      if (bVar4 == 2) {
        cVar2 = FUN_4202e0b8(param_1 + 7,2,param_2);
        return -(cVar2 + -2);
      }
    }
    cVar2 = '\x01';
  }
  else {
    if (((~bVar1 & 0x18) == 0) || ((char)bVar1 < '\0')) {
      FUN_420284c4(&DAT_3c1f71cc,"TOUCH",
                   "%s rejected wrong-device/AHT20-like frame addr=0x%02X: %02X %02X %02X %02X %02X\n"
                   ,param_2,0x2e,bVar1,param_1[1],param_1[2],param_1[3],param_1[4]);
    }
    else {
      FUN_420284c4(&DAT_3c1f71cc,"TOUCH",
                   "%s rejected invalid FT6336U status=0x%02X points=%u addr=0x%02X frame: %02X %02X %02X %02X %02X\n"
                   ,param_2,bVar1,bVar4,0x2e,bVar1,param_1[1],param_1[2],param_1[3],param_1[4]);
    }
LAB_4202e184:
    cVar2 = '\x02';
  }
  return cVar2;
}


```
## `0x4202e6a8` in `FUN_4202e6a8` at `4202e6a8`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_4202e6a8(char *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined4 uVar2;

  param_1[0] = '\0';
  param_1[1] = '\0';
  param_1[2] = '\0';
  memw();
  param_1[0x2c] = '\0';
  param_1[3] = '\0';
  param_1[4] = '\x01';
  param_1[5] = -1;
  param_1[0x1c] = '\r';
  param_1[0x1d] = '\0';
  param_1[0x1e] = '\0';
  param_1[0x1f] = '\0';
  param_1[0x20] = '\f';
  param_1[0x21] = '\0';
  param_1[0x22] = '\0';
  param_1[0x23] = '\0';
  *(undefined4 *)(param_1 + 0x24) = param_2;
  *(undefined4 *)(param_1 + 0x28) = param_3;
  FUN_420e6d08(0x2d,3);
  FUN_420e6e68(0x2d,0);
  FUN_420e95dc(500);
  FUN_420e6d08(0x2c,5);
  FUN_420e6d08(7,3);
  FUN_420e6e68(7,0);
  FUN_420e95dc(0x32);
  FUN_420e6e68(7,1);
  FUN_420e95dc(100);
  iVar1 = FUN_4202e4c4(param_1,0xd,0xc);
  *param_1 = (char)iVar1;
  if ((iVar1 != 0) && (iVar1 = FUN_4202e580(param_1), iVar1 == 0)) {
    FUN_420284c4(&DAT_3c1f71cc,"TOUCH","Mofei FT6336U config failed on SDA=%d SCL=%d addr=0x%02X\n",
                 *(undefined4 *)(param_1 + 0x1c),*(undefined4 *)(param_1 + 0x20),0x2e);
    FUN_4202e68c(param_1);
  }
  uVar2 = FUN_420e95c4();
  *(undefined4 *)(param_1 + 8) = uVar2;
  if (*param_1 != '\0') {
    FUN_420e6d08(0x2c,5);
    if (*(int *)(param_1 + 0x48) == 0) {
      iVar1 = FUN_4038afd8(&LAB_4202ec14,"MofeiTouch",0x1000,param_1,3,param_1 + 0x48,1);
      if (iVar1 == 1) {
        _DAT_3fca9704 = *(undefined4 *)(param_1 + 0x48);
        FUN_420e6fe8(0x2c,&LAB_40379670,2);
      }
      else {
        param_1[0x48] = '\0';
        param_1[0x49] = '\0';
        param_1[0x4a] = '\0';
        param_1[0x4b] = '\0';
        FUN_420284c4(&DAT_3c1f71cc,"TOUCH","Failed to create Mofei touch task\n");
      }
    }
  }
  return;
}


```

## `0x4202e96c` in `FUN_4202e96c` at `4202e96c`

```c

bool FUN_4202e96c(char *param_1,ulonglong *param_2)

{
  char cVar1;
  ushort uVar2;
  ushort uVar3;
  undefined2 uVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  uint uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  ushort *puVar11;
  ulonglong uVar12;
  char local_3a [2];
  ushort local_38;
  ushort local_36;
  undefined2 local_34;
  undefined2 local_32;
  undefined1 local_30 [48];

  if (param_2 != (ulonglong *)0x0) {
    (*(code *)&SUB_400011e8)(param_2,0,0x10);
  }
  uVar5 = FUN_420e95c4();
  FUN_421ce19c(param_1,uVar5);
  if ((*param_1 != '\x01') && (8000 < uVar5)) {
    if ((undefined *)(uVar5 - *(int *)(param_1 + 0x10)) <= &DAT_00001388) {
      return false;
    }
    *(uint *)(param_1 + 0x10) = uVar5;
    iVar6 = FUN_4202e4c4(param_1,0xd,0xc);
    *param_1 = (char)iVar6;
    if (iVar6 == 0) {
      return false;
    }
    iVar6 = FUN_4202e580(param_1);
    if (iVar6 == 0) {
      FUN_420284c4(&DAT_3c1f71cc,"TOUCH","FT6336U late-init config failed addr=0x%02X\n",0x2e);
      FUN_4202e68c(param_1);
    }
  }
  if (*param_1 == '\0') {
    return false;
  }
  iVar6 = FUN_420e6ee4(0x2c);
  if (((param_1[1] != '\x01') && (iVar6 != 0)) && (uVar5 - *(int *)(param_1 + 0x14) < 4)) {
    return false;
  }
  local_38 = 0;
  local_36 = 0;
  local_3a[0] = '\0';
  *(uint *)(param_1 + 0x14) = uVar5;
  local_34 = 0;
  local_32 = 0;
  local_3a[1] = 0;
  iVar6 = FUN_4202e848(param_1,&local_38,&local_36,&local_34,&local_32,local_3a,local_3a + 1);
  puVar11 = &local_36;
  if (iVar6 == 0) {
    if (&DAT_00001387 < (undefined *)(uVar5 - *(int *)(param_1 + 0xc))) {
      *(uint *)(param_1 + 0xc) = uVar5;
    }
    if (param_1[3] != '\0') {
      FUN_4202e68c(param_1);
      return false;
    }
    if (param_1[1] == '\0') {
      return false;
    }
    uVar8 = uVar5 - *(int *)(param_1 + 0x40);
  }
  else {
    if (param_1[2] != '\0') {
      if (local_3a[0] == '\0') {
        return false;
      }
      param_1[2] = '\0';
      return false;
    }
    uVar8 = (uint)(byte)param_1[1];
    if (local_3a[0] != '\0') {
      if (uVar8 == 0) {
        return false;
      }
      *(ushort *)(param_1 + 0x36) = local_38;
      *(ushort *)(param_1 + 0x38) = local_36;
      *(undefined2 *)(param_1 + 0x3a) = local_34;
      *(undefined2 *)(param_1 + 0x3c) = local_32;
      if (uVar5 - *(int *)(param_1 + 0x40) < 0x4b1) {
        uVar12 = FUN_4202e7b8(param_1);
        if (param_2 != (ulonglong *)0x0) {
          *param_2 = uVar12;
          *(ushort **)(param_2 + 1) = puVar11;
          *(uint *)((int)param_2 + 0xc) = uVar8;
        }
        return (uVar12 & 0xff) != 0;
      }
      param_1[1] = '\0';
      return false;
    }
    if (uVar8 == 0) {
      param_1[1] = '\x01';
      *(ushort *)(param_1 + 0x2e) = local_38;
      *(ushort *)(param_1 + 0x30) = local_36;
      *(undefined2 *)(param_1 + 0x32) = local_34;
      *(undefined2 *)(param_1 + 0x34) = local_32;
      *(uint *)(param_1 + 0x40) = uVar5;
      param_1[0x44] = '\0';
    }
    *(undefined2 *)(param_1 + 0x3a) = local_34;
    *(ushort *)(param_1 + 0x36) = local_38;
    *(ushort *)(param_1 + 0x38) = local_36;
    *(undefined2 *)(param_1 + 0x3c) = local_32;
    uVar8 = uVar5 - *(int *)(param_1 + 0x40);
    if ((param_1[0x44] == '\0') && (cVar1 = param_1[1], cVar1 != '\0')) {
      if (uVar8 < 600) {
        return false;
      }
      uVar2 = *(ushort *)(param_1 + 0x2e);
      if ((((uint)local_38 - (uint)uVar2) + 0x28 < 0x51) &&
         (uVar3 = *(ushort *)(param_1 + 0x30), ((uint)local_36 - (uint)uVar3) + 0x28 < 0x51)) {
        param_1[0x44] = '\x01';
        puVar7 = (undefined4 *)(*(code *)&SUB_400011e8)(local_30,0,0x10);
        param_1[1] = '\0';
        param_1[2] = '\x01';
        uVar4 = *(undefined2 *)(param_1 + 0x34);
        if (param_2 != (ulonglong *)0x0) {
          *(undefined2 *)((int)puVar7 + 6) = *(undefined2 *)(param_1 + 0x32);
          *(undefined1 *)puVar7 = 6;
          *(ushort *)((int)puVar7 + 2) = uVar2;
          *(ushort *)(puVar7 + 1) = uVar3;
          *(undefined2 *)(puVar7 + 2) = uVar4;
          *(undefined1 *)((int)puVar7 + 10) = 1;
          uVar10 = puVar7[1];
          uVar9 = puVar7[2];
          *(undefined4 *)param_2 = *puVar7;
          *(undefined4 *)((int)param_2 + 4) = uVar10;
          *(undefined4 *)(param_2 + 1) = uVar9;
          *(uint *)((int)param_2 + 0xc) = uVar5;
          return (bool)cVar1;
        }
        return (bool)cVar1;
      }
    }
  }
  if (0x4b0 < uVar8) {
    param_1[1] = '\0';
    param_1[2] = '\x01';
  }
  return false;
}


```
