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

extern unsigned char data_ov000_02369b2c[];
int func_02002240();
int func_02025a94();
int func_020014e8();
int func_0200160c();

void func_ov000_022de5e8(undefined4 param_1,undefined4 param_2,uint param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 auStack_48 [52];
  
  iVar2 = *((int *)data_ov000_02369b2c);
  uVar1 = func_02002240(0x34cc);
  func_02025a94(param_1,0);
  *(uint *)(iVar2 + 0xaf4) = *(uint *)(iVar2 + 0xaf4) | param_3;
  func_020014e8(auStack_48,uVar1,uVar1,uVar1);
  func_0200160c(auStack_48,param_1,param_1);
  return;
}
