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

extern unsigned char gPad[];
int TalkDemo_IsActive();
int Demo_IsSkipAreaTouched();
int PlayerDemo_StartSkip();

#pragma thumb on

void PlayerDemo_CheckSkip(int param_1)

{
  int iVar1;
  
  iVar1 = TalkDemo_IsActive();
  if (iVar1 == 0) {
    if (((*(uint *)(param_1 + 4) & 0x100000) != 0) &&
       (((*(ushort *)(((int)gPad) + 4) & 8) != 0 || (iVar1 = Demo_IsSkipAreaTouched(), iVar1 != 0)))) {
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x200000;
    }
    if ((*(uint *)(param_1 + 4) & 0x200000) != 0) {
      if ((*(uint *)(param_1 + 4) & 0x400000) == 0) {
        PlayerDemo_StartSkip(param_1);
        *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x800000;
      }
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x400000;
    }
  }
  return;
}

#pragma thumb reset
