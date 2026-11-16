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

extern unsigned char gSpecialStageMap[];
int NNS_G3dGeBufferOP_N();
int Model_Draw();

void func_ov001_0230ce38(void)

{
  int iVar1;
  undefined4 local_10 [2];
  
  iVar1 = *((int *)gSpecialStageMap);
  NNS_G3dGeBufferOP_N(0x11,0,0);
  Model_Draw(iVar1 + 0x3e4);
  local_10[0] = 1;
  NNS_G3dGeBufferOP_N(0x12,local_10);
  return;
}
