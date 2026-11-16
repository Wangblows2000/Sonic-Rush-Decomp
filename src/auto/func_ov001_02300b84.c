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



undefined4 func_ov001_02300b84(int param_1,int param_2,int param_3,int param_4)

{
  if ((((param_1 - param_3 < -0x40000) || (0x1c0000 < param_1 + param_3)) ||
      (param_2 - param_4 < -0x40000)) || (0x1c0000 < param_2 + param_4)) {
    return 1;
  }
  return 0;
}
