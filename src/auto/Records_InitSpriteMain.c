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

int ObjVram_Alloc();
int AnimatorSprite_Init();

#pragma thumb on

void Records_InitSpriteMain(int param_1,int param_2,undefined4 param_3,int param_4,undefined2 param_5,
                        undefined2 param_6)

{
  undefined4 uVar1;
  
  uVar1 = ObjVram_Alloc(0,*(undefined2 *)(param_2 + *(int *)(param_2 + 0x18) + 10));
  AnimatorSprite_Init(param_1,param_2,param_3,0,0,0,uVar1,0,0x5000200);
  *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) & 0xfff9ffff;
  *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) | param_4 << 0x11;
  *(undefined2 *)(param_1 + 0x44) = param_5;
  *(undefined2 *)(param_1 + 0x46) = param_6;
  return;
}

#pragma thumb reset
