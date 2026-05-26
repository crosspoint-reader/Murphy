# Decompile Report

## `0x420df1ee` in `FUN_420df0e8` at `420df0e8`

```c

/* WARNING: Removing unreachable block (ram,0x420df1e5) */

undefined1 FUN_420df0e8(uint param_1,char param_2)

{
  undefined4 uVar1;
  undefined4 unaff_a12;
  code *pcVar2;
  undefined4 unaff_a13;
  char *unaff_a14;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  
  iVar3 = FUN_420def30();
  if (iVar3 == 0) {
    if (s_HTTP_Version_not_supported_3c2361dd._0_4_ == param_1) {
      return 0;
    }
    if ((param_1 < (uint)s_HTTP_Version_not_supported_3c2361dd._0_4_) ||
       (s_HTTP_Version_not_supported_3c2361dd._0_4_ + 0x2a < param_1)) {
      uVar4 = (*(code *)&SUB_3fca3478)(0);
      pcVar2 = (code *)&DAT_3c237138;
      uVar1 = 1;
      goto LAB_420df115;
    }
    if (param_1 <= s_HTTP_Version_not_supported_3c2361dd._0_4_ + 1) {
      if (s_HTTP_Version_not_supported_3c2361dd._0_4_ + 1 == param_1) {
        if (param_2 == '\0') {
          uVar1 = (*(code *)&SUB_3fca3478)(0);
          FUN_420f8610(2,"Jpeg file not found",FUN_4217bd7c,uVar1,"Jpeg file not found");
          return 0;
        }
      }
      else if (param_2 == '\0') {
        return 0;
      }
      goto LAB_420df200;
    }
    unaff_a13 = (*(code *)&SUB_3fca3478)(0);
    unaff_a14 = "Jpeg file not found";
    FUN_420f8610(2,"Jpeg file not found",&DAT_3c2370b8,unaff_a13,"Jpeg file not found");
    unaff_a12 = 0x50;
    FUN_3fc9ae5f(&SUB_3c28c947,0,0x50,unaff_a13,"Jpeg file not found");
    s_HTTP_Version_not_supported_3c2361dd[4] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[5] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[6] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[7] = '\0';
    uVar1._0_1_ = '\0';
    uVar1._1_1_ = '\0';
    uVar1._2_1_ = '\0';
    uVar1._3_1_ = '\0';
    s_HTTP_Version_not_supported_3c2361dd._0_4_ = param_1;
    if (param_2 != '\0') goto LAB_420df181;
  }
  else {
    if (s_HTTP_Version_not_supported_3c2361dd._0_4_ != param_1) {
      uVar4 = (*(code *)&SUB_3fca3478)(iVar3);
      pcVar2 = FUN_4217b7e8;
      uVar1 = 2;
LAB_420df115:
      FUN_420f8610(uVar1,"Jpeg file not found",pcVar2,uVar4,"Jpeg file not found",param_1,
                   s_HTTP_Version_not_supported_3c2361dd._0_4_);
                    /* WARNING: Read-only address (ram,0x3c2361fd) is written */
      return 0;
    }
    if (param_2 != '\0') {
      return 0;
    }
LAB_420df200:
    if (param_2 != '\0') {
LAB_420df181:
      uVar4 = 0x20;
      uVar1 = FUN_420f0c98(&PTR_DAT_3c236e20,0x20,unaff_a12,unaff_a13,unaff_a14);
      goto LAB_420df18a;
    }
    iVar3 = FUN_420df9f4(iVar3);
    uVar1 = s_HTTP_Version_not_supported_3c2361dd._4_4_;
    if (iVar3 != 0) {
      uVar1 = (*(code *)&SUB_3fca3478)(iVar3);
      FUN_420f8610(2,"Jpeg file not found",&DAT_00001d90,uVar1,"Jpeg file not found");
      FUN_420f0c98(&PTR_DAT_3c236e20,0x100,&DAT_00001d90,uVar1,"Jpeg file not found");
      return 0;
    }
  }
  s_HTTP_Version_not_supported_3c2361dd._4_4_ = uVar1;
  uVar4 = 0xffffffdf;
  uVar1 = FUN_420f0cb0(&PTR_DAT_3c236e20,0xffffffdf,unaff_a12,unaff_a13,unaff_a14);
LAB_420df18a:
                    /* WARNING: Read-only address (ram,0x3c2361fd) is written */
  s_Content_Type_3c2361f8[5] = '\0';
  if (param_2 == '\0') {
    s_HTTP_Version_not_supported_3c2361dd[4] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[5] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[6] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[7] = '\0';
  }
  else {
    s_HTTP_Version_not_supported_3c2361dd._0_4_ = param_1;
    FUN_420df0b4(uVar1,uVar4,unaff_a12,unaff_a13,unaff_a14);
  }
  uVar1 = FUN_420da098(&DAT_0000ffd0,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,
                       unaff_a13,unaff_a14);
  iVar5 = FUN_4217bda0(uVar1,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,unaff_a13,
                       unaff_a14);
  for (iVar3 = 0; iVar3 != iVar5; iVar3 = iVar3 + 1) {
    iVar6 = FUN_4217be60(iVar3,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,unaff_a13
                         ,unaff_a14);
    if ((iVar6 != 0) && ((undefined *)(uint)*(ushort *)(iVar6 + 0x1c) != &DAT_3c228797)) {
      FUN_420df044(iVar6,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,unaff_a13,
                   unaff_a14);
    }
  }
  return 1;
}


```

## `0x420df0e8` in `FUN_420df0e8` at `420df0e8`

```c

/* WARNING: Removing unreachable block (ram,0x420df1e5) */

undefined1 FUN_420df0e8(uint param_1,char param_2)

{
  undefined4 uVar1;
  undefined4 unaff_a12;
  code *pcVar2;
  undefined4 unaff_a13;
  char *unaff_a14;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  
  iVar3 = FUN_420def30();
  if (iVar3 == 0) {
    if (s_HTTP_Version_not_supported_3c2361dd._0_4_ == param_1) {
      return 0;
    }
    if ((param_1 < (uint)s_HTTP_Version_not_supported_3c2361dd._0_4_) ||
       (s_HTTP_Version_not_supported_3c2361dd._0_4_ + 0x2a < param_1)) {
      uVar4 = (*(code *)&SUB_3fca3478)(0);
      pcVar2 = (code *)&DAT_3c237138;
      uVar1 = 1;
      goto LAB_420df115;
    }
    if (param_1 <= s_HTTP_Version_not_supported_3c2361dd._0_4_ + 1) {
      if (s_HTTP_Version_not_supported_3c2361dd._0_4_ + 1 == param_1) {
        if (param_2 == '\0') {
          uVar1 = (*(code *)&SUB_3fca3478)(0);
          FUN_420f8610(2,"Jpeg file not found",FUN_4217bd7c,uVar1,"Jpeg file not found");
          return 0;
        }
      }
      else if (param_2 == '\0') {
        return 0;
      }
      goto LAB_420df200;
    }
    unaff_a13 = (*(code *)&SUB_3fca3478)(0);
    unaff_a14 = "Jpeg file not found";
    FUN_420f8610(2,"Jpeg file not found",&DAT_3c2370b8,unaff_a13,"Jpeg file not found");
    unaff_a12 = 0x50;
    FUN_3fc9ae5f(&SUB_3c28c947,0,0x50,unaff_a13,"Jpeg file not found");
    s_HTTP_Version_not_supported_3c2361dd[4] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[5] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[6] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[7] = '\0';
    uVar1._0_1_ = '\0';
    uVar1._1_1_ = '\0';
    uVar1._2_1_ = '\0';
    uVar1._3_1_ = '\0';
    s_HTTP_Version_not_supported_3c2361dd._0_4_ = param_1;
    if (param_2 != '\0') goto LAB_420df181;
  }
  else {
    if (s_HTTP_Version_not_supported_3c2361dd._0_4_ != param_1) {
      uVar4 = (*(code *)&SUB_3fca3478)(iVar3);
      pcVar2 = FUN_4217b7e8;
      uVar1 = 2;
LAB_420df115:
      FUN_420f8610(uVar1,"Jpeg file not found",pcVar2,uVar4,"Jpeg file not found",param_1,
                   s_HTTP_Version_not_supported_3c2361dd._0_4_);
                    /* WARNING: Read-only address (ram,0x3c2361fd) is written */
      return 0;
    }
    if (param_2 != '\0') {
      return 0;
    }
LAB_420df200:
    if (param_2 != '\0') {
LAB_420df181:
      uVar4 = 0x20;
      uVar1 = FUN_420f0c98(&PTR_DAT_3c236e20,0x20,unaff_a12,unaff_a13,unaff_a14);
      goto LAB_420df18a;
    }
    iVar3 = FUN_420df9f4(iVar3);
    uVar1 = s_HTTP_Version_not_supported_3c2361dd._4_4_;
    if (iVar3 != 0) {
      uVar1 = (*(code *)&SUB_3fca3478)(iVar3);
      FUN_420f8610(2,"Jpeg file not found",&DAT_00001d90,uVar1,"Jpeg file not found");
      FUN_420f0c98(&PTR_DAT_3c236e20,0x100,&DAT_00001d90,uVar1,"Jpeg file not found");
      return 0;
    }
  }
  s_HTTP_Version_not_supported_3c2361dd._4_4_ = uVar1;
  uVar4 = 0xffffffdf;
  uVar1 = FUN_420f0cb0(&PTR_DAT_3c236e20,0xffffffdf,unaff_a12,unaff_a13,unaff_a14);
LAB_420df18a:
                    /* WARNING: Read-only address (ram,0x3c2361fd) is written */
  s_Content_Type_3c2361f8[5] = '\0';
  if (param_2 == '\0') {
    s_HTTP_Version_not_supported_3c2361dd[4] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[5] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[6] = '\0';
    s_HTTP_Version_not_supported_3c2361dd[7] = '\0';
  }
  else {
    s_HTTP_Version_not_supported_3c2361dd._0_4_ = param_1;
    FUN_420df0b4(uVar1,uVar4,unaff_a12,unaff_a13,unaff_a14);
  }
  uVar1 = FUN_420da098(&DAT_0000ffd0,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,
                       unaff_a13,unaff_a14);
  iVar5 = FUN_4217bda0(uVar1,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,unaff_a13,
                       unaff_a14);
  for (iVar3 = 0; iVar3 != iVar5; iVar3 = iVar3 + 1) {
    iVar6 = FUN_4217be60(iVar3,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,unaff_a13
                         ,unaff_a14);
    if ((iVar6 != 0) && ((undefined *)(uint)*(ushort *)(iVar6 + 0x1c) != &DAT_3c228797)) {
      FUN_420df044(iVar6,s___6u__E___s__u___s______s__parse_000029c1 + 0xe,unaff_a12,unaff_a13,
                   unaff_a14);
    }
  }
  return 1;
}


```

