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

extern unsigned char data_022503e0[];
int func_02060e8c();
int func_02058314();
int func_020573c0();

#pragma thumb on

void func_02057458(int param_1)

{
  int iVar1;
  
  iVar1 = func_02060e8c();
  if (iVar1 == 0) {
    if (((*(uint *)(param_1 + 4) & 0x100000) != 0) &&
       (((*(ushort *)(((int)data_022503e0) + 4) & 8) != 0 || (iVar1 = func_02058314(), iVar1 != 0)))) {
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x200000;
    }
    if ((*(uint *)(param_1 + 4) & 0x200000) != 0) {
      if ((*(uint *)(param_1 + 4) & 0x400000) == 0) {
        func_020573c0(param_1);
        *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x800000;
      }
      *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | 0x400000;
    }
  }
  return;
}

#pragma thumb reset
