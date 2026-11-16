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
extern unsigned char data_ov000_02369b84[];
int func_ov000_022f8df8();
int StageObj_Draw3DEx();

void func_ov000_022f8b0c(void)

{
  int iVar1;
  undefined1 local_18 [4];
  undefined4 local_14;
  undefined4 local_10 [2];
  
  iVar1 = *(int *)(*((int *)gTaskCurrent) + 0x10);
  func_ov000_022f8df8(*(undefined4 *)(*((int *)data_ov000_02369b84) + 0x218),&local_14,local_10,local_18);
  StageObj_Draw3DEx(local_14,local_10[0],*(undefined4 *)(iVar1 + 0x18),local_18[0],iVar1,
               *(undefined4 *)(iVar1 + 0x124));
  return;
}
