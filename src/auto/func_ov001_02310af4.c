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

int func_ov001_0231066c();
int func_ov001_0230ffd4();

void func_ov001_02310af4(int param_1)

{
  int iVar1;
  int iVar2;
  
  for (iVar2 = *(int *)(param_1 + 0xec); iVar2 != param_1 + 0xf4; iVar2 = *(int *)(iVar2 + 0xdc)) {
    func_ov001_0231066c(param_1,iVar2);
  }
  for (iVar2 = *(int *)(param_1 + 0xec); iVar2 != param_1 + 0xf4; iVar2 = *(int *)(iVar2 + 0xdc)) {
    if ((((*(ushort *)(iVar2 + 100) & 2) != 0) && (*(int *)(iVar2 + 0xd4) != 0)) &&
       ((*(ushort *)(*(int *)(iVar2 + 0xd4) + 0x60) & 1) != 0)) {
      func_ov001_0230ffd4(param_1,iVar2);
    }
    if (((((*(ushort *)(iVar2 + 100) & 0x20) != 0) && (iVar1 = *(int *)(iVar2 + 0xd8), iVar1 != 0))
        && ((iVar1 != *(int *)(iVar2 + 0xd4) || ((*(ushort *)(iVar2 + 100) & 2) == 0)))) &&
       ((*(ushort *)(iVar1 + 0x60) & 1) != 0)) {
      func_ov001_0230ffd4(param_1,iVar2);
    }
  }
  iVar2 = *(int *)(param_1 + 0xec);
  if (iVar2 == param_1 + 0xf4) {
    return;
  }
  do {
    if ((*(ushort *)(iVar2 + 0x60) & 1) != 0) {
      func_ov001_0230ffd4(param_1,iVar2);
    }
    iVar2 = *(int *)(iVar2 + 0xdc);
  } while (iVar2 != param_1 + 0xf4);
  return;
}
