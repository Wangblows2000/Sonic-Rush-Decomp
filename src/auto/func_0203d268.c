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

extern unsigned char data_022b9020[];
extern unsigned char data_022b8be0[];
int func_0203cb9c();
int func_0203d8b4();
int OS_Terminate();

void func_0203d268(int param_1)

{
  int iVar1;
  
  iVar1 = ((int)data_022b9020);
  func_0203cb9c(((int)data_022b9020),*(undefined4 *)(param_1 + 0x34));
  *(int *)(((int)data_022b8be0) + 0x1c) = param_1;
  iVar1 = func_0203d8b4(0,1,*(int *)(iVar1 + 0xafc) + *(int *)(param_1 + 0x28),
                        *(undefined4 *)(param_1 + 0x34));
  if (iVar1 == 0) {
    OS_Terminate();
    return;
  }
  return;
}
