# Decompile Report

## `0x42001cfc`

No function containing address.

## `0x42001d28`

No function containing address.

## `0x42183dd3` in `FUN_42183a68` at `42183a68`

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_42183a68(int param_1,code *param_2,code *param_3,uint param_4)

{
  byte bVar1;
  ushort uVar2;
  bool bVar3;
  short sVar4;
  code *pcVar5;
  uint uVar6;
  code *pcVar7;
  byte *pbVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  byte bVar12;
  code *pcVar13;
  byte *pbVar14;
  char *unaff_a11;
  code *unaff_a12;
  char *unaff_a13;
  uint unaff_a14;
  uint unaff_a15;
  short sVar15;
  uint uVar16;
  int iVar17;
  uint uStack_38;
  uint uStack_34;
  
  uVar6 = *(uint *)(*(int *)(param_2 + 0x10) + 0x2c) & 0xfff;
  s_E___u___s___s__incorrect_length_3c27f5b8._0_4_ =
       s_E___u___s___s__incorrect_length_3c27f5b8._0_4_ + 1;
  if (param_1 == 0) goto LAB_42183ee8;
  pcVar7 = *(code **)(param_1 + 0xe4);
  if ((pcVar7 == (code *)0x0) && (pcVar7 = *(code **)(param_1 + 0xe8), pcVar7 == (code *)0x0)) {
    FUN_42132860(1,0x3b1a87b1,1,0x8fc,0x8b2,0x39c);
    do {
                    /* WARNING: Do nothing block with infinite loop */
    } while( true );
  }
  pcVar7[0x2b8] = SUB41((uint)*(undefined4 *)(*(int *)(param_2 + 0x10) + 0x1c) >> 0x1f,0);
  if ((**(uint **)(param_2 + 0x2c) >> 5 & 1) == 0) {
    if (0xf < uVar6) {
      pbVar14 = *(byte **)(*(int *)(param_2 + 4) + 4);
      bVar12 = *pbVar14;
      uVar10 = (uint)bVar12;
      uStack_34 = uVar10 & 3;
      if ((bVar12 & 3) == 0) {
        bVar1 = pbVar14[1];
        uStack_38 = bVar1 & 3;
        uVar11 = uVar10 & 0xc;
        pcVar13 = (code *)(uVar10 & 0xfffffff0);
        pbVar8 = pbVar14 + 10;
        if ((uStack_38 == 2) && (iVar17 = FUN_3c27d718(pbVar14 + 4,pbVar14 + 0x10,6), iVar17 == 0))
        {
          uStack_34 = 1;
        }
        pcVar5 = FUN_420f9f00;
        if (((s_>>>music_set_volume__d__000017e4._0_4_ == 0) || ((bVar12 & 0xc) != 0)) ||
           (pcVar13 != (code *)0x80)) {
LAB_42183b38:
          unaff_a12 = (code *)((int)&DAT_00000004 + 2);
          unaff_a11 = (char *)(pcVar7 + 4);
          iVar17 = FUN_3c27d718(pbVar8,unaff_a11,6,unaff_a13);
          if (iVar17 != 0) {
            bVar3 = (bVar12 & 0xc) == 0;
            unaff_a12 = (code *)(uint)bVar3;
            if ((pcVar13 != (code *)((int)&DAT_000000cf + 1)) || (!bVar3)) {
              if (((DAT_80000445 & 1) == 0) || (!bVar3)) {
LAB_42183bb0:
                if (((s_>>>music_set_volume__d__000017e4._0_4_ == 0) || (pcVar13 != (code *)0x80))
                   || (!bVar3)) goto LAB_42183ece;
              }
              else {
                unaff_a13 = (char *)0x1;
                if (pcVar13 == (code *)0x80) {
                  unaff_a11 = (char *)0x0;
                }
                else {
                  unaff_a11 = (char *)(pcVar13 + -0x50);
                  if ((code *)unaff_a11 != (code *)0x0) goto LAB_42183bb0;
                }
              }
            }
          }
        }
        else {
          iVar17 = (*FUN_420f9f00)(pbVar8,0,param_3,FUN_420f9f00);
          unaff_a11 = (char *)0xfffffffd;
          unaff_a12 = param_3;
          unaff_a13 = (char *)pcVar5;
          if ((iVar17 - 2U & 0xfffffffd) != 0) goto LAB_42183b38;
        }
        *(char *)(param_1 + 0x144) = (char)param_4;
        sVar4 = (short)uVar6;
        if (((bVar12 >> 2 & 1) == 0) && ((pbVar14[4] & 1) == 0)) {
          uVar16 = FUN_3c282637(pbVar14,unaff_a11,unaff_a12,unaff_a13);
          uVar2 = *(ushort *)(pbVar14 + 0x16);
          unaff_a14 = (uint)uVar2;
          *(ushort *)(param_2 + 0x1c) = uVar2 >> 4;
          if ((bVar12 & 0xc) != 0) {
            uVar9 = 1;
            if ((uVar11 != 8) ||
               (unaff_a12 = (code *)((int)&DAT_000000bf + 1), (uVar10 & 0xffffffc0) != 0xc0))
            goto LAB_42183c90;
            unaff_a13 = (char *)FUN_42182060(pbVar14,0xc0,0xc0,unaff_a13,unaff_a14);
            *(short *)(param_2 + 0x14) = (short)unaff_a13;
            *(short *)(param_2 + 0x16) = (sVar4 + -4) - (short)unaff_a13;
            unaff_a11 = (char *)((int)&DAT_000000bf + 1);
LAB_42183d04:
            s_E___u___s___s__incorrect_length_3c27f5b8._4_4_ =
                 s_E___u___s___s__incorrect_length_3c27f5b8._4_4_ + 1;
            if ((*(int *)(param_1 + 0xe4) != 0) && ((int)unaff_a13 <= (int)uVar6)) {
              uVar11 = 8;
              if (1 < (uStack_38 - 2 & 0xff)) goto LAB_42183ece;
              iVar17 = FUN_3c282962(pcVar7,param_2,unaff_a12,unaff_a13,unaff_a14);
              unaff_a11 = (char *)param_2;
              if (iVar17 != 0) {
                return 8;
              }
              goto LAB_42183d40;
            }
            goto LAB_42183eee;
          }
          if (pcVar13 != (code *)((int)&DAT_000000cf + 1)) {
            if (pcVar13 != (code *)s_arduino_lib_builder_00000050) {
              uVar9 = 1;
              if (pcVar13 == (code *)((int)&DAT_000000ad + 3)) {
                uVar9 = uVar11;
              }
              uVar16 = 0x11;
              goto LAB_42183c90;
            }
            iVar17 = FUN_4214154c(pbVar8,uVar2 >> 4,unaff_a12,unaff_a13,unaff_a14);
            if (iVar17 != 0) {
              unaff_a15 = (uint)pbVar14[0xb];
              unaff_a14 = (uint)pbVar14[10];
              unaff_a13 = "lfs_dir_orphaningcommit";
              unaff_a12 = (code *)((int)&DAT_00000004 + 1);
              FUN_42132860(1,0x24,5,"lfs_dir_orphaningcommit",unaff_a14,unaff_a15,pbVar14[0xc],
                           pbVar14[0xd],pbVar14[0xe],pbVar14[0xf]);
              unaff_a11 = (char *)((int)&DAT_00000022 + 2);
              goto LAB_42183ece;
            }
LAB_42183c00:
            uVar16 = 0x11;
            uVar9 = 1;
LAB_42183c90:
            unaff_a11 = (char *)(pcVar7 + uVar16 + 0xf6);
            unaff_a12 = (code *)(uint)(byte)*unaff_a11;
            if (unaff_a12 == (code *)0x0) {
              if ((((uVar9 != 0) && ((pbVar14[1] >> 3 & 1) != 0)) &&
                  (*(ushort *)(pcVar7 + uVar16 * 2 + 0xd2) == unaff_a14)) &&
                 (*(code **)(param_1 + 0xe8) != pcVar7)) {
                unaff_a13 = s_wpa_crypto_funcs_expected_size___00000990 + 0x46;
                unaff_a12 = (code *)((int)&DAT_00000004 + 1);
                FUN_42132860(1,0x24,5,0x9d6,unaff_a14,uVar16);
                unaff_a11 = (char *)((int)&DAT_00000022 + 2);
                unaff_a15 = uVar16;
                goto LAB_42183ece;
              }
            }
            else {
              *unaff_a11 = '\0';
            }
            *(ushort *)(pcVar7 + uVar16 * 2 + 0xd2) = uVar2;
            goto LAB_42183cda;
          }
          pbVar8 = pbVar14 + 0x20;
          if ((pbVar14[1] >> 6 & 1) == 0) {
            pbVar8 = pbVar14 + 0x18;
          }
          if (*pbVar8 != 0x7f) goto LAB_42183c00;
          unaff_a11 = "!(hasparent && err == LFS_ERR_NOENT)";
          unaff_a12 = (code *)0x3;
          iVar17 = FUN_3c27d718(pbVar8 + 1,"!(hasparent && err == LFS_ERR_NOENT)",3,unaff_a13,
                                unaff_a14);
          if (iVar17 != 0) goto LAB_42183c00;
          if ((int)(pbVar8 + (8 - (int)pbVar14)) <= (int)uVar6) {
            sVar15 = FUN_42182060(pbVar14,"!(hasparent && err == LFS_ERR_NOENT)",3,unaff_a13,
                                  unaff_a14);
            *(short *)(param_2 + 0x14) = sVar15;
            *(short *)(param_2 + 0x16) = (sVar4 + -4) - sVar15;
            goto LAB_42183e60;
          }
        }
        else {
LAB_42183cda:
          unaff_a13 = (char *)FUN_42182060(pbVar14,unaff_a11,unaff_a12,unaff_a13,unaff_a14);
          *(short *)(param_2 + 0x14) = (short)unaff_a13;
          *(short *)(param_2 + 0x16) = (sVar4 + -4) - (short)unaff_a13;
          if (uVar11 == 4) {
            FUN_4214388c(pcVar7,param_2,pcVar13,unaff_a13,unaff_a14);
            unaff_a11 = (char *)param_2;
            unaff_a12 = pcVar13;
            goto LAB_42183ece;
          }
          if (uVar11 == 8) goto LAB_42183d04;
          if ((bVar12 & 0xc) != 0) goto LAB_42183ece;
LAB_42183e60:
          s_E___u___s___s__incorrect_length_3c27f5b8._10_2_ =
               s_E___u___s___s__incorrect_length_3c27f5b8._10_2_ + 1;
          if (uStack_34 == 0) {
            if ((bVar1 & 3) != 0) {
              unaff_a13 = (char *)0x20100018;
              unaff_a12 = (code *)((int)&DAT_00000004 + 1);
              FUN_42132860(1,0x24,5,0x20100018,uStack_38);
              uVar11 = 0;
              unaff_a11 = (char *)((int)&DAT_00000022 + 2);
              unaff_a14 = uStack_38;
              goto LAB_42183ece;
            }
            if ((0x17 < uVar6) &&
               (((DAT_00001974 == 0 || ((pbVar14[4] & 1) == 0)) ||
                (iVar17 = FUN_421439b8(param_2,unaff_a11,unaff_a12,unaff_a13,unaff_a14), iVar17 == 0
                )))) {
              FUN_42143f04(param_1,param_2,pcVar13,param_3,param_4);
              unaff_a11 = (char *)param_2;
              unaff_a12 = pcVar13;
              unaff_a13 = (char *)param_3;
              unaff_a14 = param_4;
            }
          }
        }
        uVar11 = 0;
        goto LAB_42183ece;
      }
    }
LAB_42183ee8:
    uVar11 = 0xff;
    goto LAB_42183ece;
  }
  pcVar13 = (code *)0x80;
  pbVar14 = *(byte **)(*(int *)(param_2 + 4) + 4);
  uStack_38 = pbVar14[1] & 3;
  unaff_a13 = (char *)FUN_42182060(pbVar14);
  uStack_34 = 0;
LAB_42183d40:
  if (uStack_38 == 2) {
    uVar10 = pbVar14[1] & 0x40;
    if ((pbVar14[1] & 0x40) == 0) {
      *(uint *)(*(int *)(param_2 + 4) + 4) =
           *(int *)(*(int *)(param_2 + 4) + 4) + (uint)*(ushort *)(param_2 + 0x14);
    }
    else {
      uVar10 = FUN_421324c8(pcVar7,param_2,unaff_a13,unaff_a13,unaff_a14);
      unaff_a11 = (char *)param_2;
      unaff_a12 = (code *)unaff_a13;
      if (uVar10 == 0) goto LAB_42183eee;
    }
    if (pcVar13 == (code *)0x80) {
      bVar12 = pbVar14[0x18];
    }
    else {
      bVar12 = 0;
    }
    unaff_a12 = (code *)FUN_42137f9c(param_2,pbVar14,uVar6,unaff_a13,unaff_a14);
    if (unaff_a12 == (code *)0x0) {
      return 8;
    }
    iVar17 = FUN_42142b88(param_1,pcVar7,unaff_a12,unaff_a13,unaff_a14);
    param_2 = unaff_a12;
    unaff_a11 = (char *)pcVar7;
    if (iVar17 != 1) {
      if ((uVar10 == 0) && (uVar11 = 8, DAT_0000196d != '\x01')) goto LAB_42183ece;
      if ((char)bVar12 < '\0') {
        s_E___u___s___s__incorrect_length_3c27f5b8._18_2_ =
             s_E___u___s___s__incorrect_length_3c27f5b8._18_2_ + 1;
        param_2 = (code *)FUN_42137ce0(param_1,unaff_a12,unaff_a12,unaff_a13,unaff_a14);
        pcVar7 = unaff_a12;
        if (param_2 == (code *)0x0) {
          return 8;
        }
      }
      if ((s_>>>music_set_volume__d__000017e4._0_4_ != 0) &&
         ((*(uint *)(param_2 + 0x1c) >> 0x10 & 1) != 0)) {
        if (*(int *)(param_1 + 0x98) == 5) {
          iVar17 = (*(code *)&SUB_02ba3737)(&SUB_02ba3737,pcVar7,unaff_a12,unaff_a13,unaff_a14);
          unaff_a12 = (code *)0x0;
          (*_DAT_25282125)(param_1 + 0x18,iVar17 * 1000,0,unaff_a13,unaff_a14);
        }
        iVar17 = *(int *)(*(int *)(param_2 + 4) + 4);
        if (_DAT_600ffc08 != (code *)0x0) {
          unaff_a14 = (uint)*(ushort *)(param_2 + 0x1c);
          unaff_a12 = (code *)(uint)*(ushort *)(param_2 + 0x16);
          unaff_a13 = (char *)(iVar17 + 6);
          (*_DAT_600ffc08)(0,iVar17,unaff_a12,unaff_a13,unaff_a14);
        }
        FUN_0000131a(param_2,iVar17,unaff_a12,unaff_a13,unaff_a14);
        return 8;
      }
      unaff_a11 = (char *)pcVar7;
      if (DAT_00020001 != (code *)0x0) {
        uVar2 = *(ushort *)(param_2 + 0x16);
        uVar11 = 8;
        FUN_4218443c(*(undefined4 *)(*(int *)(param_2 + 4) + 4),(code *)(uint)uVar2,unaff_a12,
                     unaff_a13,unaff_a14);
        unaff_a11 = (char *)(uint)uVar2;
        if (uStack_34 == 0) {
          (*DAT_00020001)(*(undefined4 *)(*(int *)(param_2 + 4) + 4),*(undefined2 *)(param_2 + 0x16)
                          ,param_2,unaff_a13,unaff_a14);
          return 8;
        }
        goto LAB_42183ece;
      }
    }
  }
LAB_42183eee:
  uVar11 = 8;
LAB_42183ece:
  FUN_0000131a(param_2,unaff_a11,unaff_a12,unaff_a13,unaff_a14,unaff_a15);
                    /* WARNING: Read-only address (ram,0x420f9f04) is written */
  return uVar11;
}


```

## `0x42000660`

No function containing address.

## `0x420e5371`

No function containing address.

