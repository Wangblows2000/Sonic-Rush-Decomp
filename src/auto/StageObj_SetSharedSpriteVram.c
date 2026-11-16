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

int ObjVramRef_Acquire();

void StageObj_SetSharedSpriteVram(int param_1,uint param_2,int param_3)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = 0xffff;
  *(int *)(param_1 + 0x90) = param_3;
  if ((param_2 == uVar1) && (*(int **)(param_1 + 0x8c) != (int *)0x0)) {
    iVar3 = **(int **)(param_1 + 0x8c);
    param_2 = (uint)*(ushort *)(iVar3 + *(int *)(iVar3 + 0x18) + 8);
  }
  uVar2 = ObjVramRef_Acquire(param_3,0,param_2);
  *(undefined4 *)(param_1 + 200) = uVar2;
  uVar2 = ObjVramRef_Acquire(param_3 + 8,1,param_2);
  *(undefined4 *)(param_1 + 0x110) = uVar2;
  return;
}
