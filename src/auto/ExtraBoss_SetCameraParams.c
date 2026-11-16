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

int ExtraBoss_GetWork();

void ExtraBoss_SetCameraParams(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4
                        ,undefined4 param_5)

{
  int iVar1;
  
  iVar1 = ExtraBoss_GetWork();
  *(undefined4 *)(iVar1 + 0x324) = param_1;
  *(undefined4 *)(iVar1 + 0x328) = param_2;
  *(undefined4 *)(iVar1 + 0x32c) = param_3;
  *(undefined4 *)(iVar1 + 0x330) = param_4;
  *(undefined4 *)(iVar1 + 0x334) = param_5;
  return;
}
