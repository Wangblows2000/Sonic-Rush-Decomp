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

extern unsigned char data_ov000_0236a48c[];
int func_ov000_022f1d20();
int func_ov000_022f4ce4();

void func_ov000_022f1d88(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  func_ov000_022f4ce4(param_1,0x1c,0x1000,0);
  iVar1 = ((int)data_ov000_0236a48c);
  *(undefined4 *)(((int)data_ov000_0236a48c) + 0x48) = 0x14000;
  *(undefined4 *)(iVar1 + 0x4c) = 0x32000;
  *(undefined2 *)(iVar1 + 0x6c) = 0x80;
  *(undefined2 *)(iVar1 + 0x30) = 0x80;
  *(undefined4 *)(iVar1 + 0x74) = 0;
  uVar2 = ((int)func_ov000_022f1d20);
  *(undefined4 *)(iVar1 + 0x38) = 0;
  *(undefined4 *)(param_1 + 0x68) = uVar2;
  (**(code **)(param_1 + 0x68))(param_1);
  return;
}
