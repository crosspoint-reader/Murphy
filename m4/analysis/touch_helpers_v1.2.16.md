# Decompile Report

## `0x4202e4c4` in `FUN_4202e4c4` at `4202e4c4`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_4202e4c4(int param_1,uint param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  undefined **ppuVar3;
  undefined1 auStack_2f [11];
  int local_24;

  memw();
  memw();
  local_24 = _DAT_3fcab97c;
  if ((-1 < (int)(param_2 | param_3)) && (param_2 != param_3)) {
    *(uint *)(param_1 + 0x1c) = param_2;
    *(uint *)(param_1 + 0x20) = param_3;
    ppuVar3 = &PTR_PTR_3fca1900;
    if (*(char *)(param_1 + 4) == '\0') {
      ppuVar3 = &PTR_PTR_3fca190c;
    }
    iVar1 = (**(code **)*ppuVar3)(ppuVar3,param_2,param_3);
    if (iVar1 != 0) {
      (*(code *)&SUB_400011e8)(auStack_2f,0,0xb);
      iVar1 = FUN_4202e4a0(param_1,2,auStack_2f,0xb);
      if ((iVar1 != 0) && (iVar2 = FUN_4202e140(auStack_2f,"detect"), iVar2 == 1))
      goto LAB_4202e549;
      ppuVar3 = &PTR_PTR_3fca1900;
      if (*(char *)(param_1 + 4) == '\0') {
        ppuVar3 = &PTR_PTR_3fca190c;
      }
      (**(code **)(*ppuVar3 + 4))();
    }
  }
  iVar1 = 0;
LAB_4202e549:
  memw();
  memw();
  if (local_24 != _DAT_3fcab97c) {
    FUN_403875b8();
  }
  return iVar1;
}


```
## `0x4202e580` in `FUN_4202e580` at `4202e580`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_4202e580(undefined4 param_1)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  bool bVar7;
  int iVar8;
  bool bVar9;
  char local_33 [15];
  int local_24;

  memw();
  memw();
  local_24 = _DAT_3fcab97c;
  bVar1 = FUN_4202e560(param_1,0,0);
  bVar2 = FUN_4202e560(param_1,0x80,0x16);
  bVar3 = FUN_4202e560(param_1,0x88,4);
  local_33[0] = -1;
  bVar4 = FUN_4202e4a0(param_1,0,local_33,1);
  local_33[1] = 0;
  FUN_4202e4a0(param_1,0xa6,local_33 + 1,1);
  local_33[2] = '\0';
  bVar5 = FUN_4202e4a0(param_1,0x80,local_33 + 2,1);
  local_33[3] = '\0';
  bVar6 = FUN_4202e4a0(param_1,0x88,local_33 + 3,1);
  (*(code *)&SUB_400011e8)(local_33 + 4,0,0xb);
  iVar8 = FUN_4202e4a0(param_1,2,local_33 + 4,0xb);
  bVar9 = false;
  if (iVar8 != 0) {
    iVar8 = FUN_4202e140(local_33 + 4,"config");
    bVar9 = iVar8 == 1;
  }
  bVar7 = false;
  if (((bVar1 & bVar2) != 0) && (bVar7 = false, (bVar3 & bVar4) != 0)) {
    bVar7 = false;
    if (((bVar9 & bVar5 & bVar6) != 0) &&
       ((bVar7 = false, local_33[0] == '\0' && (local_33[2] == '\x16')))) {
      bVar7 = local_33[3] == '\x04';
    }
  }
  memw();
  memw();
  if (local_24 != _DAT_3fcab97c) {
    FUN_403875b8();
  }
  return bVar7;
}


```

## `0x4202e68c` in `FUN_4202e68c` at `4202e68c`

```c

void FUN_4202e68c(undefined4 *param_1)

{
  undefined **ppuVar1;

  *param_1 = 0;
  ppuVar1 = &PTR_PTR_3fca1900;
  if (*(char *)(param_1 + 1) == '\0') {
    ppuVar1 = &PTR_PTR_3fca190c;
  }
  (**(code **)(*ppuVar1 + 4))();
  return;
}


```

## `0x4202e848` in `FUN_4202e848` at `4202e848`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_4202e848(int param_1,ushort *param_2,ushort *param_3,ushort *param_4,ushort *param_5,
                undefined1 *param_6,undefined1 *param_7)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  ushort uVar4;
  undefined1 uVar5;
  undefined1 uVar6;
  ushort uVar7;
  undefined4 in_t0;
  int iStack_5c;
  byte local_2f;
  byte bStack_2e;
  byte bStack_2d;
  byte bStack_2c;
  byte bStack_2b;
  int iStack_24;

  memw();
  memw();
  iStack_24 = _DAT_3fcab97c;
  (*(code *)&SUB_400011e8)(&local_2f,0,0xb);
  *(undefined1 *)(param_1 + 3) = 0;
  iStack_5c = FUN_4202e4a0(param_1,2,&local_2f,0xb);
  uVar3 = 0;
  if (iStack_5c != 0) {
    iVar2 = FUN_4202e140(&local_2f,"read");
    uVar3 = 0;
    uVar5 = 1;
    if (iVar2 == 1) goto LAB_4202e8b8;
    uVar3 = 2;
  }
  *(char *)(param_1 + 3) = (char)(uVar3 >> 1);
  iStack_5c = 0;
  while( true ) {
    memw();
    memw();
    if (iStack_24 == _DAT_3fcab97c) break;
    FUN_403875b8();
    uVar5 = (undefined1)in_t0;
LAB_4202e8b8:
    bVar1 = bStack_2e;
    uVar6 = (char)uVar3;
    if ((local_2f & 0xf) != 0) {
      uVar6 = uVar5;
    }
    *param_7 = uVar6;
    if ((local_2f & 0xf) == 0) {
      if (*(byte *)(param_1 + 5) != local_2f) {
        *(byte *)(param_1 + 5) = local_2f;
      }
      *param_6 = 1;
      *param_2 = *(ushort *)(param_1 + 0x36);
      *param_3 = *(ushort *)(param_1 + 0x38);
      *param_4 = *(ushort *)(param_1 + 0x3a);
      *param_5 = *(ushort *)(param_1 + 0x3c);
    }
    else {
      *(byte *)(param_1 + 5) = local_2f;
      uVar7 = (ushort)bStack_2d | (bStack_2e & 0xf) << 8;
      uVar4 = (ushort)bStack_2b | (bStack_2c & 0xf) << 8;
      FUN_421ce12c(param_1);
      *param_2 = uVar7;
      *param_3 = uVar4;
      *param_4 = uVar7;
      *param_5 = uVar4;
      param_7 = (undefined1 *)((bVar1 >> 6) - 1);
      if (param_7 != (undefined1 *)0x0) {
        uVar5 = (char)uVar3;
      }
      *param_6 = uVar5;
    }
  }
  return iStack_5c;
}


```

## `0x4202ec14`

No function containing address.

## `0x4202e0b8` in `FUN_4202e0b8` at `4202e0b8`

```c

/* WARNING: Control flow encountered bad instruction data */

undefined4 FUN_4202e0b8(byte *param_1,undefined4 param_2,undefined4 param_3)

{
  byte bVar1;

  bVar1 = *param_1;
  if (bVar1 >> 6 != 3) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  FUN_420284c4(&DAT_3c1f71cc,"TOUCH",
               "%s rejected invalid FT6336U p%u addr=0x%02X event=%u raw=%u,%u bytes: %02X %02X %02X %02X\n"
               ,param_3,param_2,0x2e,3,(bVar1 & 0xf) << 8 | (uint)param_1[1],
               (param_1[2] & 0xf) << 8 | (uint)param_1[3],(uint)bVar1,(uint)param_1[1],
               (uint)param_1[2],(uint)param_1[3]);
  return 0;
}


```
