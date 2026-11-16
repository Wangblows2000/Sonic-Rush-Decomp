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

int func_02015640();
int func_0201564c();
int func_0201565c();

undefined4 func_0201544c(int param_1,int param_2)

{
  int *piVar1;
  
  piVar1 = (int *)func_02015640();
  if ((*piVar1 == 4) || (*piVar1 == 6)) {
    return 0;
  }
  func_0201564c(param_1 + -0xe);
  func_0201565c(param_2 + -0xe);
  return 1;
}
