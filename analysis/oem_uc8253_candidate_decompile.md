# Decompile Report

## `0x0012ebdc` in `FUN_0012ebdc` at `0012ebdc`

```c

int FUN_0012ebdc(int param_1)

{
  char cVar1;
  byte bVar2;
  char *pcVar3;
  int iVar4;
  undefined2 *puVar5;
  undefined1 uVar6;
  undefined4 uVar7;
  int iVar8;
  undefined4 uVar9;
  
  iVar4 = DAT_000601a4;
  *(undefined1 *)(param_1 + 2) = 1;
  puVar5 = DAT_00060530;
  iVar8 = DAT_000601ec;
  pcVar3 = DAT_0006018c;
  if (*(char *)(iVar4 + 0xe) == '\0') {
    cVar1 = *(char *)(DAT_000601ec + 0x12);
    if (cVar1 != '\b') {
      uVar7 = *(undefined4 *)(DAT_00060340 + (uint)*(byte *)(iVar4 + 0x11) * 600 + 0x1bc);
      *(undefined1 *)(DAT_000601ec + 0xf) = 2;
      *(undefined1 *)(iVar8 + 0x10) = 1;
      *(char *)(iVar8 + 0x15) = cVar1;
      FUN_0013aa00(iVar8,uVar7,2,1,cVar1);
      goto LAB_0012ed60;
    }
LAB_0012ed21:
    FUN_0012df3c(param_1,0x16,0);
  }
  else {
    bVar2 = *(byte *)(DAT_000601ec + 0x12);
    if (bVar2 == 4) {
      uVar6 = 2;
    }
    else {
      if (4 < bVar2) {
        if (bVar2 == 6) {
          FUN_0012df3c(param_1,0x1b,0);
          goto LAB_0012ed60;
        }
        if (bVar2 < 6) {
          FUN_0012df3c(param_1,0x11,0);
          goto LAB_0012ed60;
        }
        if (bVar2 == 7) {
          if (*DAT_00060410 < 10) {
            uVar7 = *(undefined4 *)(DAT_00060340 + (uint)*(byte *)(iVar4 + 0x11) * 600 + 0x1ac);
            *(undefined1 *)(DAT_000601ec + 0xf) = 2;
            *(undefined1 *)(iVar8 + 0x10) = 1;
            *(undefined1 *)(iVar8 + 0x15) = 7;
            FUN_0013aa00(iVar8,uVar7,2,1,iVar8);
          }
          else if (*DAT_0006018c == '\0') {
            do {
              iVar8 = FUN_0012865c(1,0);
              if (iVar8 == 0) goto LAB_0012ed60;
              iVar8 = FUN_0012809c(*puVar5,0);
            } while (iVar8 == 0);
            FUN_0012df3c(param_1,0x21,0);
            uVar7 = DAT_00060538;
            uVar9 = FUN_001237d0(DAT_00060538,0x21,0);
            cVar1 = *pcVar3;
            FUN_00134238(uVar7,uVar9,cVar1);
            FUN_00134314(*(undefined1 *)(iVar4 + 0x13),uVar9,cVar1);
          }
          else {
            FUN_0012df3c(param_1,0x21,1);
          }
          goto LAB_0012ed60;
        }
        if (bVar2 != 8) goto LAB_0012ed60;
        goto LAB_0012ed21;
      }
      if (bVar2 == 1) {
        FUN_0012df3c(param_1,0x14,0);
        goto LAB_0012ed60;
      }
      if (bVar2 == 0) {
        FUN_0012df3c(param_1,0xf,0);
        goto LAB_0012ed60;
      }
      if (bVar2 != 2) {
        if (bVar2 == 3) {
          FUN_0012df3c(DAT_00060140,0x1f,0);
        }
        goto LAB_0012ed60;
      }
      uVar6 = 8;
    }
    *(undefined1 *)(param_1 + 0x13) = uVar6;
    FUN_0012df3c(param_1,3,0);
  }
LAB_0012ed60:
  *(undefined1 *)(param_1 + 2) = 0;
  return param_1;
}


```

## `0x0012ed68` in `FUN_0012ed68` at `0012ed68`

```c

uint FUN_0012ed68(uint param_1)

{
  char cVar1;
  byte bVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  
  uVar7 = DAT_00060778;
  iVar6 = DAT_00060460;
  iVar5 = DAT_000601ec;
  iVar4 = DAT_000601a4;
  uVar3 = DAT_00060140;
  cVar1 = *(char *)(DAT_000601ec + 0x12);
  if (cVar1 == '\x01') {
    *(undefined1 *)(DAT_000601a4 + 0x17) = 2;
    *(undefined1 *)(iVar5 + 0xf) = 2;
    *(undefined1 *)(iVar5 + 0x10) = 0x13;
    *(undefined1 *)(iVar5 + 0x15) = 1;
    FUN_0013aa00(iVar5,uVar7,1,1,iVar4);
    uVar3 = DAT_00060774;
    param_1 = DAT_000602a0;
    FUN_0014bfd4(DAT_000602a0,0x5c8,DAT_00060774,0x100,iVar4);
    FUN_0014beec(param_1,0x5c8,uVar3,0x100,iVar4);
  }
  else if (cVar1 == '\0') {
    bVar2 = *DAT_0006045c;
    param_1 = (uint)*(byte *)(DAT_000601ec + 0x23);
    *(byte *)(DAT_000601ec + 0x22) = *(byte *)(DAT_000601ec + 0x23);
    FUN_0012df3c(DAT_00060140,3,*(undefined1 *)(iVar6 + (uint)bVar2 * 2));
  }
  else if (cVar1 == '\x02') {
    param_1 = (uint)*(byte *)(DAT_000601ec + 0x23);
    *(byte *)(DAT_000601ec + 0x22) = *(byte *)(DAT_000601ec + 0x23);
    FUN_0012df3c(uVar3,1,0);
  }
  else if (cVar1 == '\x03') {
    FUN_0012df3c(DAT_00060140,0x13,0);
  }
  return param_1;
}


```

## `0x0012f968` in `FUN_0012f968` at `0012f968`

```c

uint FUN_0012f968(uint param_1)

{
  char cVar1;
  char cVar2;
  byte bVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 unaff_a13;
  undefined4 unaff_a14;
  undefined4 uVar6;
  uint uVar7;
  int iVar8;
  undefined4 uVar9;
  
  iVar4 = DAT_000601ec;
  cVar1 = *(char *)(DAT_000601ec + 0x12);
  cVar2 = *(char *)(DAT_000601ec + 0x10);
  if (cVar1 == '\t') {
    if (cVar2 == '\x06') goto LAB_0012f9b4;
  }
  else {
    if (cVar1 == '\n') {
      if (*(char *)(DAT_000601a4 + 0x1a) == '\0') {
        *(undefined1 *)(DAT_000601a4 + 0x1a) = 1;
      }
      FUN_0012f644(param_1,0x16);
      return param_1;
    }
    if (cVar1 == '\b') {
      if (*(char *)(DAT_000601a4 + 0x1a) != '\0') {
        *(undefined1 *)(DAT_000601a4 + 0x1a) = 0;
        uVar6 = FUN_0012df3c(param_1,0x22,0);
        FUN_00122f98(uVar6,0x22,0);
        return 0;
      }
      *(undefined1 *)(DAT_000601a4 + 0x1a) = 1;
      FUN_00122f70();
      FUN_0012f644(param_1,0x16);
      return param_1;
    }
    uVar7 = FUN_00122c14();
    iVar8 = FUN_00122f64(uVar7);
    uVar9 = DAT_0006079c;
    uVar6 = DAT_00060798;
    if (iVar8 != 0) {
      *(undefined1 *)(iVar4 + 0x15) = *(undefined1 *)(iVar4 + 0x12);
      *(undefined1 *)(param_1 + 0x15) = 3;
      *(undefined1 *)(iVar4 + 0xf) = 0x20;
      *(undefined1 *)(iVar4 + 0x10) = 0x22;
      *(undefined1 *)(iVar4 + 0x14) = 1;
      FUN_0013ccf0(iVar4,uVar6,0,iVar8);
      return 1;
    }
    if ((uVar7 & 0xff) <= (uint)*(byte *)(iVar4 + 0x12)) {
      return param_1;
    }
    FUN_0013aafc(iVar4,DAT_0006079c,2,0,0);
    iVar8 = FUN_00122d14(*(undefined1 *)(iVar4 + 0x12),uVar9,2,0,0);
    uVar6 = DAT_000607a0;
    if (iVar8 == 0) {
      bVar3 = *(byte *)(iVar4 + 0x12);
      if (cVar2 == '\x06') {
        *(undefined1 *)(iVar4 + 0xf) = 2;
        *(undefined1 *)(iVar4 + 0x10) = 6;
        *(byte *)(iVar4 + 0x15) = bVar3;
        FUN_0013aa00(iVar4,uVar6,2,1,0);
        return (uint)bVar3;
      }
      *(undefined1 *)(iVar4 + 0xf) = 2;
      *(undefined1 *)(iVar4 + 0x10) = 0x16;
      *(byte *)(iVar4 + 0x15) = bVar3;
      FUN_0013aa00(iVar4,uVar6,2,1,0);
      return (uint)bVar3;
    }
    uVar6 = FUN_00122c20(*(undefined1 *)(iVar4 + 0x12),uVar9,2,0,0);
    uVar5 = DAT_000603b0;
    (*DAT_000604a8)(DAT_000603b0,uVar6,2,0,0);
    uVar9 = FUN_00122c48(*(undefined1 *)(iVar4 + 0x12),uVar6,2,0,0);
    uVar6 = DAT_00060110;
    (*DAT_000604a8)(DAT_00060110,uVar9,2,0,0);
    (*DAT_00060120)(DAT_000603b4,uVar5,2,0,0);
    uVar9 = DAT_000602a0;
    FUN_0014bfd4(DAT_000602a0,0x468,uVar5,0x20,0);
    unaff_a13 = 0x12;
    FUN_0014bfd4(uVar9,0x488,uVar6,0x12,0);
    FUN_0014beec(uVar9,0x488,uVar6,0x12,0);
    if (cVar2 == '\x06') {
      unaff_a14 = 0;
LAB_0012f9b4:
      FUN_0012df3c(param_1,6,1,unaff_a13,unaff_a14);
      return param_1;
    }
    unaff_a14 = 0;
  }
  FUN_0012df3c(param_1,0x16,0,unaff_a13,unaff_a14);
  return param_1;
}


```

## `0x00131898` in `FUN_00131898` at `00131898`

```c

int FUN_00131898(undefined4 param_1)

{
  uint *puVar1;
  int iVar2;
  byte *pbVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  undefined4 uVar8;
  
  uVar4 = DAT_000602a0;
  puVar1 = DAT_000601a4;
  *DAT_000601a4 = 0;
  puVar1[1] = 0;
  puVar1[2] = 0;
  puVar1[3] = 0;
  puVar1[4] = 0;
  puVar1[5] = 0;
  puVar1[6] = 0;
  puVar1[7] = 0;
  puVar1[8] = 0;
  puVar1[9] = 0;
  FUN_0014bf98(uVar4,0,puVar1,0x28);
  pbVar3 = DAT_0006029c;
  uVar8 = 0x60;
  FUN_0014bf98(uVar4,0x60,DAT_0006029c,2);
  uVar6 = *puVar1 ^ puVar1[1] ^ puVar1[2] ^ puVar1[3] ^ puVar1[4] ^ puVar1[5] ^ puVar1[6] ^
          puVar1[7] ^ puVar1[8] ^ puVar1[9];
  uVar7 = uVar6 >> 0x10 & 0xff;
  if (((uint)*pbVar3 != ((uVar6 ^ uVar6 >> 8 & 0xff ^ uVar7 ^ uVar6 >> 0x18) & 0xff)) &&
     (0xfd < (byte)(pbVar3[1] - 1))) {
    FUN_00131834(param_1,0x60,pbVar3,2);
    uVar8 = DAT_00060828;
    uVar7 = FUN_001841b8(DAT_00060224,DAT_00060828,pbVar3,2);
  }
  FUN_00125704(uVar7,uVar8,pbVar3,2);
  uVar5 = DAT_000603b0;
  (*DAT_00060168)(DAT_000603b0,0,0x20,2);
  uVar8 = DAT_00060110;
  (*DAT_00060168)(DAT_00060110,0,0x12,2);
  FUN_0014bf98(uVar4,0x468,uVar5,0x20);
  FUN_0014bf98(uVar4,0x488,uVar8,0x12);
  uVar5 = DAT_00060830;
  uVar4 = DAT_0006082c;
  *DAT_00060788 = (ushort)*(byte *)((int)puVar1 + 0x16);
  FUN_0015ab7c(uVar5,uVar4,uVar8,0x12);
  iVar2 = DAT_000601ec;
  *(undefined1 *)(DAT_000601ec + 0xb) = 0xe;
  *(undefined1 *)(iVar2 + 9) = 0xe;
  return iVar2;
}


```

## `0x00131994` in `FUN_00131994` at `00131994`

```c

uint FUN_00131994(uint param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int iVar7;
  
  uVar3 = DAT_000602c4;
  iVar2 = DAT_000601ec;
  if (*(char *)(DAT_000601ec + 0x12) == '\0') {
    cVar1 = *(char *)(param_1 + 0x15);
    if (cVar1 == '\x01') {
      FUN_0013084c(param_1,*(undefined1 *)(param_1 + 0x16),2);
      FUN_0012df3c(DAT_00060140,0x1f,*(undefined1 *)(iVar2 + 0x15));
    }
    else if (cVar1 == '\x02') {
      uVar6 = FUN_00131834(param_1);
      FUN_0012555c(uVar6);
      (*DAT_000601b4)(DAT_0006037c);
      puVar4 = DAT_00060368;
      param_1 = DAT_000602a0;
      *DAT_00060368 = DAT_00060380;
      *(undefined2 *)(puVar4 + 1) = 0x11;
      FUN_0014bfd4(param_1,0x5b8,puVar4,6);
      FUN_0014beec(param_1,0x5b8,puVar4,6);
      uVar6 = DAT_000603ac;
      (*DAT_00060168)(DAT_000603ac,0,0x10,6);
      FUN_0014bfd4(param_1,0x420,uVar6,0x10);
      FUN_0014beec(param_1,0x420,uVar6,0x10);
      FUN_0018386c(DAT_000603bc,0x420,uVar6,0x10);
    }
    else if (cVar1 == '\x04') {
      (*DAT_00060120)(DAT_00060834,DAT_000602c4);
      iVar7 = (*DAT_00060840)(DAT_00060194,uVar3);
      uVar5 = DAT_0006083c;
      uVar6 = DAT_00060838;
      if (iVar7 == 0) {
        param_1 = (uint)*(byte *)(iVar2 + 0x12);
        *(undefined1 *)(iVar2 + 0xf) = 2;
        *(undefined1 *)(iVar2 + 0x10) = 3;
        *(byte *)(iVar2 + 0x15) = *(byte *)(iVar2 + 0x12);
        FUN_0013aa00(iVar2,uVar5,2,1);
      }
      else {
        *(undefined1 *)(iVar2 + 0xf) = 2;
        *(undefined1 *)(iVar2 + 0x10) = 3;
        *(undefined1 *)(iVar2 + 0x15) = *(undefined1 *)(iVar2 + 0x12);
        FUN_0013aa00(iVar2,uVar6,1,1);
        FUN_00126524(param_1,0,*(undefined1 *)(iVar2 + 0x15),0);
      }
    }
    else if (cVar1 == '\x03') {
      FUN_00122c70();
      FUN_0012f644(param_1,0x16);
    }
  }
  else if (*(char *)(DAT_000601ec + 0x12) == '\x02') {
    if (*(char *)(param_1 + 0x15) == '\x01') {
      FUN_0013084c(param_1,*(undefined1 *)(param_1 + 0x16),2);
      param_1 = DAT_000602c4;
      (*DAT_00060494)(DAT_00060194,DAT_000602c4,2);
      FUN_0012689c(param_1,param_1,2);
      FUN_0012df3c(DAT_00060140,0x1f,*(undefined1 *)(iVar2 + 0x15));
    }
  }
  else {
    FUN_0012df3c(param_1,*(undefined1 *)(DAT_000601ec + 0x10),0);
  }
  return param_1;
}


```

## `0x0013381c` in `FUN_0013381c` at `0013381c`

```c

int FUN_0013381c(void)

{
  char *pcVar1;
  short *psVar2;
  undefined4 uVar3;
  int iVar4;
  int unaff_a11;
  char cVar5;
  undefined1 uVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  
  FUN_00183e44(DAT_00060224);
  FUN_00183e44(DAT_000609f0);
  FUN_00183e44(DAT_000609f4);
  iVar4 = DAT_00060140;
  FUN_00124e20(DAT_00060140);
  uVar7 = FUN_0018640c(0x50);
  cVar5 = (*DAT_000609fc)(uVar7);
  pcVar1 = DAT_00060454;
  *DAT_00060454 = cVar5;
  psVar2 = DAT_00060788;
  if ((cVar5 == '\x02') && (*DAT_00060788 != 0)) {
    FUN_0018640c(0x50);
    iVar8 = FUN_001337b4(*psVar2 * 100);
    if (iVar8 == 0) {
      FUN_00125168(iVar4,1);
      (*DAT_000603dc)(*DAT_000603d4,0);
      iVar4 = DAT_000603d8;
      iVar8 = (0x3d - (uint)*DAT_000602bc) * DAT_000603d8;
      unaff_a11 = iVar8 >> 0x1f;
      uVar7 = (*DAT_000603e0)(iVar8,unaff_a11);
      (*DAT_000603e4)(uVar7,unaff_a11);
    }
  }
  FUN_0018640c(0xf0,unaff_a11);
  uVar3 = DAT_000609f8;
  uVar7 = DAT_000602a0;
  FUN_0014bc1c(DAT_000602a0,DAT_000609f8);
  FUN_00131898(iVar4,uVar3);
  uVar3 = DAT_00060810;
  *(undefined4 *)(iVar4 + 0x18) = 0;
  FUN_0014bf98(uVar7,0x460,uVar3,4);
  FUN_00125d48(iVar4,0x460,uVar3,4);
  FUN_001259ec(iVar4,0x460,uVar3,4);
  FUN_00125ab0(iVar4,0x460,uVar3,4);
  FUN_00126074(iVar4,0x460,uVar3,4);
  *DAT_000607c0 = 3;
  *DAT_0006018c = 0;
  iVar8 = DAT_000601ec;
  uVar7 = FUN_00138a3c(DAT_000601ec,0x460,uVar3,4);
  FUN_00133600(uVar7,0x460,uVar3,4);
  FUN_00138c70(iVar8,1,uVar3,4);
  if (*pcVar1 != '\0') {
    uVar7 = FUN_0012e8f8(iVar4,1,uVar3,4);
    uVar7 = FUN_00134308(uVar7,1,uVar3,4);
    uVar7 = FUN_00133504(uVar7,1,uVar3,4);
    FUN_00136f20(uVar7,1,uVar3,4);
    FUN_00138c7c(iVar8,1,uVar3,4);
    FUN_00138b50(iVar8,1,uVar3,4);
    return iVar4;
  }
  uVar7 = *(undefined4 *)(iVar4 + 0x18);
  *DAT_000603b8 = 0;
  uVar6 = FUN_00131284(iVar4,uVar7,uVar3,4);
  *(undefined1 *)(DAT_000601a4 + 0xe) = uVar6;
  FUN_00125c20(iVar4,uVar7,uVar3,4);
  FUN_00125168(iVar4,1,uVar3,4);
  iVar9 = FUN_001251dc(iVar4,1,uVar3,4);
  if (iVar9 == 0) {
    FUN_00125200(iVar4,1,uVar3,4);
  }
  *(undefined1 *)(iVar8 + 0x22) = *(undefined1 *)(iVar8 + 0x23);
  FUN_00138b50(iVar8,0,uVar3,4);
  uVar7 = FUN_0012df3c(iVar4,1,4,4);
                    /* WARNING: Subroutine does not return */
  FUN_001873a8(uVar7,1,4,4);
}


```

## `0x0012aff4` in `FUN_0012aff4` at `0012aff4`

```c

byte * FUN_0012aff4(uint param_1,undefined4 param_2)

{
  byte bVar1;
  byte *pbVar2;
  uint uVar3;
  uint uVar4;
  byte bStack_22;
  byte bStack_21;
  byte abStack_20 [32];
  
  pbVar2 = (byte *)0x7f;
  if (param_1 < 0x80) {
    FUN_001249b4(param_2,param_1);
  }
  else {
    if ((param_1 >> 6 & 0xffff) < 0x20) {
      pbVar2 = &bStack_22;
    }
    else {
      uVar4 = (int)(param_1 >> 6) >> 6;
      uVar3 = uVar4 & 0x3ff;
      bVar1 = (byte)uVar3;
      if ((uVar4 & 0x3f0) == 0) {
        bStack_22 = bVar1 | 0xe0;
        pbVar2 = &bStack_21;
      }
      else {
        bStack_22 = bVar1 & 0x3f | 0x80;
        bStack_21 = (byte)((int)uVar3 >> 6) | 0xf0;
        pbVar2 = abStack_20;
      }
    }
    while( true ) {
      pbVar2 = pbVar2 + -1;
      if (*pbVar2 == 0) break;
      FUN_001249b4(param_2,*pbVar2);
    }
  }
  return pbVar2;
}


```

