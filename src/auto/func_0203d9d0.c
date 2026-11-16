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

extern unsigned char data_022c14a0[];
int func_0203da24();
int OS_CreateThread();
int OS_WakeupThreadDirect();

void func_0203d9d0(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = ((int)data_022c14a0);
  iVar2 = ((int)data_022c14a0) + 0x400;
  *(undefined2 *)(((int)data_022c14a0) + 0x494) = 0;
  OS_CreateThread(iVar2,((int)func_0203da24),iVar1,iVar2,0x400,0xf);
  OS_WakeupThreadDirect(iVar1 + 0x400);
  return;
}
