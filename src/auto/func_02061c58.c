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

extern unsigned char data_020776a8[];
int func_02061a08();

#pragma thumb on

undefined4 func_02061c58(undefined4 *param_1,int param_2)

{
  undefined2 uVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)(((int)data_020776a8) + param_2 * 0x14);
  param_1[0xd] = 0;
  if (param_2 == 1) {
    *param_1 = 0;
    return 1;
  }
  uVar1 = (undefined2)0x100;
  *(undefined2 *)(param_1 + 0xb) = uVar1;
  *(undefined2 *)((int)param_1 + 0x2e) = uVar1;
  *(undefined2 *)(param_1 + 0xc) = 0;
  *param_1 = *puVar2;
  param_1[9] = (int)*(short *)(puVar2 + 1) << 8;
  param_1[10] = (int)*(short *)((int)puVar2 + 6) << 8;
  param_1[3] = (int)*(short *)(puVar2 + 2) << 8;
  param_1[4] = (int)*(short *)((int)puVar2 + 10) << 8;
  param_1[5] = (int)*(short *)(puVar2 + 3) << 8;
  param_1[6] = (int)*(short *)((int)puVar2 + 0xe) << 8;
  param_1[7] = (int)*(short *)(puVar2 + 4);
  param_1[8] = (int)*(short *)((int)puVar2 + 0x12);
  func_02061a08();
  return 1;
}

#pragma thumb reset
