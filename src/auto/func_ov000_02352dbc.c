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

int func_ov000_02352cb0();
int func_ov000_0234c9e8();
int func_ov000_0234a064();
int func_ov000_0234c4a4();
int func_0204b7ec();

void func_ov000_02352dbc(int param_1,undefined4 param_2,int param_3,int param_4)

{
  undefined4 uVar1;
  
  func_ov000_0234c9e8();
  func_ov000_0234a064(param_1,0);
  *(undefined4 *)(param_1 + 1000) = param_2;
  func_ov000_0234c4a4(param_1,0x30);
  uVar1 = ((int)func_ov000_02352cb0);
  *(ushort *)(param_1 + 0x5c) = *(ushort *)(param_1 + 0x5c) | 4;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  *(uint *)(param_1 + 0x58) = *(uint *)(param_1 + 0x58) | 0x10;
  *(uint *)(param_1 + 0x58) = *(uint *)(param_1 + 0x58) | 0x2000;
  *(uint *)(param_1 + 0x58) = *(uint *)(param_1 + 0x58) | 0x100;
  *(uint *)(param_1 + 0x58) = *(uint *)(param_1 + 0x58) & 0xffffff7f;
  *(uint *)(param_1 + 0x58) = *(uint *)(param_1 + 0x58) & 0xffffffbf;
  *(uint *)(param_1 + 0x32c) = *(uint *)(param_1 + 0x32c) | 0x100000;
  *(int *)(param_1 + 0x3f8) = (int)*(short *)(param_1 + 0x34);
  *(undefined2 *)(param_1 + 0x2e) = 0;
  *(undefined2 *)(param_1 + 0x30) = 0;
  *(undefined2 *)(param_1 + 0x28) = 0;
  *(undefined2 *)(param_1 + 0x2a) = 0;
  *(undefined2 *)(param_1 + 0x34) = 0;
  *(undefined1 *)(param_1 + 1) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(int *)(param_1 + 0x3f0) = param_3 << 8;
  *(int *)(param_1 + 0x3f4) = param_4 << 8;
  *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 0x10);
  *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0x14);
  *(int *)(param_1 + 0x10) = *(int *)(*(int *)(param_1 + 1000) + 0x10) + *(int *)(param_1 + 0x3f0);
  *(int *)(param_1 + 0x14) = *(int *)(*(int *)(param_1 + 1000) + 0x14) + *(int *)(param_1 + 0x3f4);
  func_0204b7ec(param_1 + 900,0,0x5b);
  return;
}
