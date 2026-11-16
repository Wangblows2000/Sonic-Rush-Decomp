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

int Records_DrawName();

#pragma thumb on

void Records_Unk_ov001_022ed14c(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  if (0 < *(int *)(param_1 + 0x28)) {
    iVar3 = 0x3d;
    do {
      iVar1 = iVar3 - *(int *)(param_1 + 0x30);
      if ((0x2d < iVar1) && (iVar1 < 0x7d)) {
        Records_DrawName(param_1,0x18,iVar1,iVar2 + 1);
      }
      iVar3 = iVar3 + 0x10;
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x28));
  }
  return;
}

#pragma thumb reset
