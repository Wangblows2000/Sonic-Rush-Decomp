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



void ExtraBoss_StopAndFaceCenter2(int param_1)

{
  *(undefined4 *)(param_1 + 0x60) = 0;
  *(undefined4 *)(param_1 + 0x5c) = *(undefined4 *)(param_1 + 0x60);
  if (*(int *)(param_1 + 0x18) < 0xbb000) {
    *(undefined4 *)(param_1 + 0x54) = 0x1000;
  }
  else {
    *(undefined4 *)(param_1 + 0x54) = 0xfffff000;
  }
  return;
}
