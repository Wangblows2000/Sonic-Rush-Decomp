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

int Marker_State_Unk_ov000_02310574();
int StageObj_SetAnimation();
int func_ov000_0232b020();

void func_ov000_023106b8(int param_1)

{
  undefined4 uVar1;
  
  StageObj_SetAnimation(param_1,0);
  uVar1 = ((int)Marker_State_Unk_ov000_02310574);
  *(ushort *)(param_1 + 0x5c) = *(ushort *)(param_1 + 0x5c) | 4;
  *(ushort *)(param_1 + 0x5c) = *(ushort *)(param_1 + 0x5c) | 0x10;
  *(uint *)(param_1 + 0x54) = *(uint *)(param_1 + 0x54) | 0x10;
  *(uint *)(param_1 + 0x54) = *(uint *)(param_1 + 0x54) | 2;
  *(undefined4 *)(param_1 + 0x68) = uVar1;
  *(undefined4 *)(param_1 + 8) = 0;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined2 *)(param_1 + 0x2e) = 0;
  if (*(int *)(param_1 + 0x10) < 0x20000) {
    func_ov000_0232b020();
    return;
  }
  return;
}
