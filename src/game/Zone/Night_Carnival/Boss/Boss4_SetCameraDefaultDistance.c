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

void Boss4_SetCameraDefaultDistance(void)

{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0x1befb8;
  iVar1 = ((int)data_ov000_0236a48c);
  *(undefined4 *)(((int)data_ov000_0236a48c) + 0x84) = 0x50000;
  *(undefined4 *)(iVar1 + 0x78) = uVar2;
  *(undefined2 *)(iVar1 + 0x80) = 0x80;
  *(undefined2 *)(iVar1 + 0x8c) = 0x80;
  return;
}
