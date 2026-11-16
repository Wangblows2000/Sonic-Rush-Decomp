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
extern unsigned char gDisplayMain[];
int func_ov000_022ef4ec();
int func_ov000_022c5c80();
int Sound_PlaySeqArc();

void func_ov000_022ef528(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = ((int)data_ov000_0236a48c);
  *(undefined4 *)(((int)data_ov000_0236a48c) + 0x78) = 0x60000;
  *(undefined2 *)(iVar1 + 0x80) = 0x1000;
  *(undefined4 *)(iVar1 + 0x84) = 0;
  *(undefined2 *)(iVar1 + 0x8c) = 0x1000;
  *(undefined2 *)(iVar1 + 0x8e) = 0;
  *(undefined2 *)(iVar1 + 0x92) = 0x1000;
  func_ov000_022c5c80(6);
  *(undefined2 *)(((int)gDisplayMain) + 0x48) = 0x10;
  Sound_PlaySeqArc(0,0,0x7c);
  uVar2 = ((int)func_ov000_022ef4ec);
  *(undefined2 *)(param_1 + 0x434) = 0;
  *(undefined4 *)(param_1 + 0x68) = uVar2;
  return;
}
