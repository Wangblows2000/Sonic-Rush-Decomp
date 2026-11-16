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

int SceneChange_Unk_02055044();
int SceneChange_Unk_02055028();
int func_ov001_022cee04();
int func_ov001_022cedc0();

#pragma thumb on

void func_ov001_022e9664(undefined4 param_1)

{
  switch(param_1) {
  case 0:
    SceneChange_Unk_02055044();
    return;
  case 1:
    SceneChange_Unk_02055028();
    return;
  case 2:
    func_ov001_022cee04();
    return;
  case 3:
    func_ov001_022cedc0();
  }
  return;
}

#pragma thumb reset
