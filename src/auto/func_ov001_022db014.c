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

extern unsigned char gTaskCurrent[];
int func_ov001_022dac0c();
int func_ov001_022db150();
int func_ov001_022da59c();
int func_ov001_022d9cb0();
int func_02050e50();
int func_0205002c();
int func_ov001_022da41c();
int func_ov001_022da374();
int func_ov001_022da33c();
int func_ov001_022da2cc();
int func_ov001_022da288();
int func_02040ec0();
int func_ov001_022d6dc0();
int func_ov001_022d97a8();
int func_02051db0();
int func_02050018();
int func_0205000c();
int func_02050e70();
int func_02051070();
int func_ov001_022da850();

#pragma thumb on

void func_ov001_022db014(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(*((int *)gTaskCurrent) + 0x10);
  func_ov001_022da59c(piVar2);
  func_ov001_022d9cb0(piVar2,0,0);
  *piVar2 = *piVar2 + 1;
  iVar1 = func_02050e50();
  if (iVar1 != 0) {
    *piVar2 = 0;
    piVar2[1] = 0;
    func_0205002c();
    func_ov001_022da41c(piVar2);
    func_ov001_022da374(piVar2);
    func_ov001_022da33c(piVar2);
    func_ov001_022da2cc(piVar2,0);
    func_ov001_022da2cc(piVar2,1);
    func_ov001_022da288(piVar2,0);
    func_ov001_022da288(piVar2,1);
    func_02040ec0((int)piVar2 + 0xb44,0xe);
    func_02040ec0((int)piVar2 + 0xb70,0xf);
    func_02040ec0((int)piVar2 + 0xb9c,0x10);
    func_02040ec0((int)piVar2 + 0xbc8,0x11);
    func_02040ec0((int)piVar2 + 0xbf4,0x12);
    func_02040ec0((int)piVar2 + 0xc20,0x13);
    func_ov001_022d6dc0();
    func_ov001_022d97a8(0,0x10,0x10,0xc);
    func_02051db0(2,0xc,0x3000,0);
    *(undefined4 *)(*((int *)gTaskCurrent) + 8) = ((int)func_ov001_022dac0c);
    return;
  }
  iVar1 = func_02050018();
  if (iVar1 != 0) {
    *piVar2 = 0;
    piVar2[1] = 0;
    func_ov001_022da41c(piVar2);
    iVar1 = func_0205000c();
    if (iVar1 == 0) {
      func_02050e70();
      func_02051070();
      func_ov001_022da33c(piVar2);
      func_ov001_022da850(piVar2);
      return;
    }
    func_ov001_022da33c(piVar2);
    *(undefined4 *)(*((int *)gTaskCurrent) + 8) = ((int)func_ov001_022db150);
  }
  return;
}

#pragma thumb reset
