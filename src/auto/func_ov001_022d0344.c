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

int func_ov001_022d02f4();
int func_02033550();
int func_ov001_022d1404();

#pragma thumb on

void func_ov001_022d0344(int param_1)

{
  undefined4 uVar1;
  uint *puVar2;
  uint *puVar3;
  uint *puVar4;
  
  puVar2 = (uint *)(param_1 + 0xc10);
  *puVar2 = *puVar2 | 4;
  puVar3 = (uint *)(param_1 + 0xc58);
  *puVar3 = *puVar3 | 4;
  puVar4 = (uint *)(param_1 + 0xca0);
  *puVar4 = *puVar4 | 4;
  *puVar2 = *puVar2 & 0xffffff7f;
  *puVar3 = *puVar3 & 0xffffff7f;
  *puVar4 = *puVar4 & 0xffffff7f;
  uVar1 = 0x78000;
  *(undefined4 *)(param_1 + 0xcc8) = 0x78000;
  *(undefined4 *)(param_1 + 0xcd4) = 0;
  *(undefined4 *)(param_1 + 0xcec) = 0;
  *(undefined4 *)(param_1 + 0xccc) = uVar1;
  *(undefined4 *)(param_1 + 0xcd8) = 0x1000;
  *(undefined4 *)(param_1 + 0xcf0) = 0;
  func_02033550(param_1 + 0xbe4,0x1c);
  func_02033550(param_1 + 0xc2c,0x1f);
  func_ov001_022d1404(param_1,((int)func_ov001_022d02f4));
  return;
}

#pragma thumb reset
