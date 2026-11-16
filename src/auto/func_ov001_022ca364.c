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

int func_02033550();
int func_0203344c();
int func_02032d2c();

#pragma thumb on

void func_ov001_022ca364(int param_1,undefined2 param_2,undefined2 param_3,int param_4,
                        ushort param_5)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0x3ac) + 0x1838 + param_4 * 0x48;
  iVar1 = *(int *)(param_1 + 0x3ac) + 0x1b98;
  if ((uint)param_5 != *(uint *)(iVar1 + param_4 * 4)) {
    func_02033550(iVar2);
    *(uint *)(iVar1 + param_4 * 4) = (uint)param_5;
  }
  *(undefined2 *)(iVar2 + 4) = param_2;
  *(undefined2 *)(iVar2 + 6) = param_3;
  func_0203344c(iVar2,0,0);
  func_02032d2c(iVar2);
  return;
}

#pragma thumb reset
