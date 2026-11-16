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

extern unsigned char data_02089254[];
extern unsigned char data_02089264[];
int func_02000cb4();
int func_02003d94();

#pragma thumb on

void func_02047458(int param_1)

{
  undefined1 auStack_18 [16];
  
  func_02000cb4(auStack_18);
  func_02003d94(((int)data_02089254),auStack_18,0,0,0,*(int *)(param_1 + 8) >> 0xc);
  func_02003d94(((int)data_02089264),auStack_18,0,0,0,*(int *)(param_1 + 4) >> 0xc);
  return;
}

#pragma thumb reset
