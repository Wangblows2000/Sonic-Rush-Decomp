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

int StaffRoll_PictureState_Blaze7();
int func_ov001_022d0220();
int StaffRoll_SetPictureState();

#pragma thumb on

void StaffRoll_PictureState_Blaze6(int param_1)

{
  if (*(int *)(param_1 + 0xcc4) <= *(int *)(param_1 + 0xcbc) + 0x14000) {
    *(undefined4 *)(param_1 + 0xcdc) = 0;
    *(undefined4 *)(param_1 + 0xcf4) = 0;
  }
  func_ov001_022d0220(param_1);
  if ((*(uint *)(param_1 + 0xc58) & 2) != 0) {
    StaffRoll_SetPictureState(param_1,((int)StaffRoll_PictureState_Blaze7));
  }
  return;
}

#pragma thumb reset
