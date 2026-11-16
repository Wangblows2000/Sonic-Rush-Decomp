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
int func_ov001_022cc17c();
int func_ov001_022cbf30();
int func_ov001_022cde84();
int func_ov001_022cde60();
int OptionsMenu_Action_TimeLimit();
int OptionsMenu_Action_SoundTest();
int OptionsMenu_Action_EraseData();
int func_ov001_022cdda0();

#pragma thumb on

void OptionsMenu_State_EnterItem(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(*((int *)gTaskCurrent) + 0x10);
  iVar1 = func_ov001_022cc17c(iVar2);
  func_ov001_022cbf30(iVar2);
  *(int *)(iVar2 + 0x14) = *(int *)(iVar2 + 0x14) + 1;
  if (iVar1 != 0) {
    *(undefined4 *)(iVar2 + 0x10) = 1;
    switch(*(undefined4 *)(iVar2 + *(int *)(iVar2 + 4) * 0x30 + 0x28c)) {
    case 0:
      func_ov001_022cde84();
      return;
    case 1:
      func_ov001_022cde60();
      return;
    case 2:
      OptionsMenu_Action_TimeLimit();
      return;
    case 3:
      OptionsMenu_Action_SoundTest();
      return;
    case 4:
      OptionsMenu_Action_EraseData();
      return;
    case 5:
      func_ov001_022cdda0();
    }
  }
  return;
}

#pragma thumb reset
