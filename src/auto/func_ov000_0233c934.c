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

extern unsigned char data_ov000_023689a0[];
extern unsigned char data_ov000_0236aa8c[];
int func_02021310();
int func_02021264();
int func_020212e0();

void func_ov000_0233c934(int *param_1,undefined1 *param_2)

{
  int iVar1;
  undefined1 auStack_68 [92];
  
  func_02021310(auStack_68,((int *)data_ov000_023689a0),*((int *)data_ov000_0236aa8c));
  iVar1 = func_02021264(auStack_68,param_2[1]);
  *param_1 = iVar1 + *(int *)(iVar1 + 0x10) + 4;
  func_020212e0(auStack_68);
  param_1[1] = (int)param_2;
  *(undefined1 *)(param_1 + 2) = *param_2;
  *(undefined2 *)((int)param_1 + 10) = *(undefined2 *)(param_2 + 6);
  return;
}
