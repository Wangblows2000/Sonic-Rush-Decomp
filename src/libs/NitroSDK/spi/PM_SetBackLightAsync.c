/* ghidra_tier.py: mechanically fixed Ghidra draft, verified by match.py. */
typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined3;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned char bool;
typedef unsigned short ushort;
typedef unsigned short word;
typedef unsigned int uint;
typedef unsigned int dword;
typedef unsigned long ulong;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef int code();
#define true 1
#define false 0

int PM_SendUtilityCommandAsync();

undefined4 PM_SetBackLightAsync(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = 0;
  if (param_1 == 0) {
    if (param_2 == 1) {
      iVar2 = 6;
    }
    if (param_2 == 0) {
      iVar2 = 7;
    }
  }
  else if (param_1 == 1) {
    if (param_2 == 1) {
      iVar2 = 4;
    }
    if (param_2 == 0) {
      iVar2 = 5;
    }
  }
  else if (param_1 == 2) {
    if (param_2 == 1) {
      iVar2 = 8;
    }
    if (param_2 == 0) {
      iVar2 = 9;
    }
  }
  if (iVar2 == 0) {
    return 0xffff;
  }
  uVar1 = PM_SendUtilityCommandAsync(iVar2,param_3,param_4);
  return uVar1;
}
