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

int func_ov000_0233b178();
int func_ov000_0233b158();
int func_ov000_023558bc();
int Sound_PlaySeqArcEx();

void func_ov000_023557c8(int param_1,int param_2,int param_3,int param_4,short param_5)

{
  *(uint *)(param_1 + 0x330) = *(uint *)(param_1 + 0x330) & 0xffffffaf;
  *(uint *)(param_1 + 0x330) = *(uint *)(param_1 + 0x330) & 0xffffff5f;
  func_ov000_0233b178(*(undefined1 *)(param_1 + 0x327));
  func_ov000_0233b158(*(undefined1 *)(param_1 + 0x327));
  *(undefined1 *)(param_1 + 1) = 0;
  if (param_2 != 0) {
    *(int *)(param_1 + 0x10) = param_2;
  }
  if (param_3 != 0) {
    *(int *)(param_1 + 0x14) = param_3;
  }
  func_ov000_023558bc(param_1,param_4,(int)param_5);
  if (param_4 == 0) {
    *(undefined2 *)(param_1 + 0x2e) = 0;
  }
  *(undefined2 *)(param_1 + 0x352) = 0x1e;
  *(undefined4 *)(param_1 + 0xc) = 0x1e;
  *(uint *)(param_1 + 0x58) = *(uint *)(param_1 + 0x58) & 0xffffff7f;
  Sound_PlaySeqArcEx(param_1 + 900,0,0x41);
  return;
}
