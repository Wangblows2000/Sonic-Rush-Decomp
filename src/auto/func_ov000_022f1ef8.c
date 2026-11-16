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

extern unsigned char data_0208fe70[];
int func_ov000_022f1df0();
int func_ov000_022f4ce4();

void func_ov000_022f1ef8(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  func_ov000_022f4ce4(param_1,0x1b,0x1000,1);
  iVar2 = *((int *)data_0208fe70) * 0x196225 + 0x3c6ef35f;
  *((int *)data_0208fe70) = iVar2;
  uVar1 = ((int)func_ov000_022f1df0);
  *(ushort *)(param_1 + 0x434) = ((ushort)((uint)iVar2 >> 0x10) & 0x3f) + 0x3c;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  (**(code **)(param_1 + 0x68))(param_1);
  return;
}
