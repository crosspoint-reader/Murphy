# Decompile Report

## `0x4202e4a0` in `FUN_4202e4a0` at `4202e4a0`

```c

undefined4 FUN_4202e4a0(int param_1,undefined1 param_2,undefined4 param_3,undefined1 param_4)

{
  undefined4 uVar1;
  undefined **ppuVar2;

  ppuVar2 = &PTR_PTR_3fca1900;
  if (*(char *)(param_1 + 4) == '\0') {
    ppuVar2 = &PTR_PTR_3fca190c;
  }
  uVar1 = (**(code **)(*ppuVar2 + 8))(ppuVar2,param_2,param_3,param_4);
  return uVar1;
}


```
## `0x4202e560` in `FUN_4202e560` at `4202e560`

```c

undefined4 FUN_4202e560(int param_1,undefined1 param_2,undefined1 param_3)

{
  undefined4 uVar1;
  undefined **ppuVar2;

  ppuVar2 = &PTR_PTR_3fca1900;
  if (*(char *)(param_1 + 4) == '\0') {
    ppuVar2 = &PTR_PTR_3fca190c;
  }
  uVar1 = (**(code **)(*ppuVar2 + 0xc))(ppuVar2,param_2,param_3);
  return uVar1;
}


```
