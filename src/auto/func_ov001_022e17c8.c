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

extern unsigned char data_ov001_0231c428[];
int File_Load();

#pragma thumb on

void func_ov001_022e17c8(int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = 0;
  puVar2 = ((void *)data_ov001_0231c428);
  do {
    uVar1 = File_Load(*puVar2,0);
    *(undefined4 *)(param_1 + 0xeac) = uVar1;
    puVar2 = puVar2 + 1;
    param_1 = param_1 + 4;
    iVar3 = iVar3 + 1;
  } while (iVar3 < 5);
  return;
}

#pragma thumb reset
