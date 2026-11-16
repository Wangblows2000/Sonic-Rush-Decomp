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

extern unsigned char data_ov001_0231f524[];
extern unsigned char data_ov001_0231f520[];
int func_ov001_02300c54();
int func_ov001_022ffce8();

void func_ov001_02305564(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = func_ov001_02300c54();
  iVar2 = func_ov001_022ffce8();
  iVar3 = *(int *)(iVar1 + 0x18) - *(int *)(iVar2 + 0x31c);
  iVar1 = *(int *)(iVar1 + 0x1c) - *(int *)(iVar2 + 800);
  if (iVar3 < 0) {
    if (iVar3 < -0x7800) {
      iVar3 = -0x7800;
    }
    *(int *)(iVar2 + 0x31c) = *(int *)(iVar2 + 0x31c) + iVar3;
  }
  else if (0 < iVar3) {
    if (0x7800 < iVar3) {
      iVar3 = 0x7800;
    }
    *(int *)(iVar2 + 0x31c) = *(int *)(iVar2 + 0x31c) + iVar3;
  }
  if (iVar1 < 0) {
    if (iVar1 < -0x7800) {
      iVar1 = -0x7800;
    }
    *(int *)(iVar2 + 800) = *(int *)(iVar2 + 800) + iVar1;
  }
  else if (0 < iVar1) {
    if (0x7800 < iVar1) {
      iVar1 = 0x7800;
    }
    *(int *)(iVar2 + 800) = *(int *)(iVar2 + 800) + iVar1;
  }
  *((int *)data_ov001_0231f524) = -(*(int *)(iVar2 + 0x31c) + -0x80000);
  *((int *)data_ov001_0231f520) = -(*(int *)(iVar2 + 800) + -0x60000);
  return;
}
