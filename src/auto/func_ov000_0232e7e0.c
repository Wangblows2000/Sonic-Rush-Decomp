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

extern unsigned char gCamera[];
int func_ov000_0232e854();
int func_ov000_022c5e80();

void func_ov000_0232e7e0(int param_1)

{
  undefined1 auStack_18 [4];
  int local_14;
  int local_10 [2];
  
  func_ov000_0232e854();
  func_ov000_022c5e80(*(undefined4 *)(((int)gCamera) + 0xe0),*(undefined4 *)(((int)gCamera) + 0xe4),
                      *(int *)(param_1 + 0x10) << 4,&local_14,local_10,auStack_18);
  *(int *)(param_1 + 0x10) = local_14 >> 4;
  *(int *)(param_1 + 0x18) = local_10[0] >> 4;
  return;
}
