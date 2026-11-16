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

int StaffRoll_PictureState_BlazeExit3();
int StaffRoll_SetPictureState();

#pragma thumb on

void StaffRoll_PictureState_BlazeExit2(int param_1)

{
  if (0x6e000 < *(int *)(param_1 + 0xcc0)) {
    *(undefined4 *)(param_1 + 0xcd4) = 0xfffff000;
    *(undefined4 *)(param_1 + 0xcec) = 0;
    *(undefined4 *)(param_1 + 0xcd8) = 0;
    *(undefined4 *)(param_1 + 0xcf0) = 0;
    StaffRoll_SetPictureState(param_1,((int)StaffRoll_PictureState_BlazeExit3));
  }
  return;
}

#pragma thumb reset
