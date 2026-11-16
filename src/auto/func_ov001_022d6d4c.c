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

extern unsigned char data_02087ccc[];
extern unsigned char data_02087cb8[];
int func_ov001_022d6cf4();
int func_ov001_022d5114();
int func_ov001_022d6c98();
int func_ov001_022d6bf0();
int func_ov001_022d6b5c();
int func_ov001_022d68d4();
int func_ov001_022d5888();
int func_02037d60();
int func_ov001_022d6630();
int func_ov001_022d587c();

#pragma thumb on

void func_ov001_022d6d4c(void)

{
  int iVar1;
  
  iVar1 = func_ov001_022d6cf4();
  func_ov001_022d5114();
  func_ov001_022d6c98(iVar1);
  func_ov001_022d6bf0(iVar1);
  func_ov001_022d6b5c(iVar1);
  func_ov001_022d68d4(iVar1);
  func_ov001_022d5888(iVar1);
  func_02037d60();
  func_ov001_022d6630(*(undefined4 *)(iVar1 + 0x48c));
  func_ov001_022d587c(iVar1);
  *((int *)data_02087ccc) = 0;
  *((int *)data_02087cb8) = *((int *)data_02087cb8) & 0xffffffef;
  return;
}

#pragma thumb reset
