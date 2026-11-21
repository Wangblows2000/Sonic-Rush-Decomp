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

int Records_DrawTwoDigits();

#pragma thumb on

void Records_DrawRecordTimes(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int local_1c;
  
  local_1c = 0;
  if ((0 < *(int *)(param_1 + 0x28)) && (0 < *(int *)(param_1 + 0x28))) {
    iVar3 = 0x38;
    iVar2 = param_1;
    do {
      iVar1 = iVar3 - *(int *)(param_1 + 0x30);
      if ((0x28 < iVar1) && (iVar1 < 0x78)) {
        Records_DrawTwoDigits(param_1,0x68,iVar1,*(undefined1 *)(iVar2 + 0x3c),2);
        Records_DrawTwoDigits(param_1,0x90,iVar1,*(undefined1 *)(iVar2 + 0x3d),2);
        Records_DrawTwoDigits(param_1,0xb8,iVar1,*(undefined1 *)(iVar2 + 0x3e),2);
      }
      iVar3 = iVar3 + 0x10;
      iVar2 = iVar2 + 4;
      local_1c = local_1c + 1;
    } while (local_1c < *(int *)(param_1 + 0x28));
  }
  return;
}

#pragma thumb reset
