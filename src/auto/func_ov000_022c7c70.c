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

extern unsigned char data_ov000_02369acc[];
int func_ov000_022c7970();
int func_ov000_022c7be4();

void func_ov000_022c7c70(int param_1)

{
  int *piVar1;
  
  piVar1 = ((int *)data_ov000_02369acc);
  *(undefined2 *)(param_1 + 0x20c) = 0;
  switch(*(undefined2 *)(*piVar1 + 0x22a)) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    goto LAB_022c7ca8;
  case 3:
LAB_022c7ca8:
    *(undefined4 *)(param_1 + 0x68) = ((int)func_ov000_022c7970);
    return;
  }
  *(undefined4 *)(param_1 + 0x68) = ((int)func_ov000_022c7be4);
  return;
}
