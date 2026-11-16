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
int VsMenu_State_Unk_ov001_022dac0c();
int VsMenu_State_Unk_ov001_022db150();
int VsMenu_Unk_ov001_022da59c();
int VsMenu_Unk_ov001_022d9cb0();
int WirelessLobbyChild_IsFinished();
int MsgBox_Destroy();
int VsMenu_Unk_ov001_022da41c();
int VsMenu_Unk_ov001_022da374();
int VsMenu_Unk_ov001_022da33c();
int VsMenu_Unk_ov001_022da2cc();
int VsMenu_Unk_ov001_022da288();
int KeyframeAnim_SetAnim();
int TitleMenu_Unk_ov001_022d6dc0();
int VsMenu_CreateBrightnessFade();
int SysSound_PlaySEDelayed();
int MsgBox_IsClosed();
int MsgBox_GetResult();
int WirelessLobbyChild_RequestRestart();
int WirelessLobbyChild_RequestCancel();
int VsMenu_Unk_ov001_022da850();

#pragma thumb on

void VsMenu_State_Unk_ov001_022db014(void)

{
  int iVar1;
  int *piVar2;
  
  piVar2 = *(int **)(*((int *)gTaskCurrent) + 0x10);
  VsMenu_Unk_ov001_022da59c(piVar2);
  VsMenu_Unk_ov001_022d9cb0(piVar2,0,0);
  *piVar2 = *piVar2 + 1;
  iVar1 = WirelessLobbyChild_IsFinished();
  if (iVar1 != 0) {
    *piVar2 = 0;
    piVar2[1] = 0;
    MsgBox_Destroy();
    VsMenu_Unk_ov001_022da41c(piVar2);
    VsMenu_Unk_ov001_022da374(piVar2);
    VsMenu_Unk_ov001_022da33c(piVar2);
    VsMenu_Unk_ov001_022da2cc(piVar2,0);
    VsMenu_Unk_ov001_022da2cc(piVar2,1);
    VsMenu_Unk_ov001_022da288(piVar2,0);
    VsMenu_Unk_ov001_022da288(piVar2,1);
    KeyframeAnim_SetAnim((int)piVar2 + 0xb44,0xe);
    KeyframeAnim_SetAnim((int)piVar2 + 0xb70,0xf);
    KeyframeAnim_SetAnim((int)piVar2 + 0xb9c,0x10);
    KeyframeAnim_SetAnim((int)piVar2 + 0xbc8,0x11);
    KeyframeAnim_SetAnim((int)piVar2 + 0xbf4,0x12);
    KeyframeAnim_SetAnim((int)piVar2 + 0xc20,0x13);
    TitleMenu_Unk_ov001_022d6dc0();
    VsMenu_CreateBrightnessFade(0,0x10,0x10,0xc);
    SysSound_PlaySEDelayed(2,0xc,0x3000,0);
    *(undefined4 *)(*((int *)gTaskCurrent) + 8) = ((int)VsMenu_State_Unk_ov001_022dac0c);
    return;
  }
  iVar1 = MsgBox_IsClosed();
  if (iVar1 != 0) {
    *piVar2 = 0;
    piVar2[1] = 0;
    VsMenu_Unk_ov001_022da41c(piVar2);
    iVar1 = MsgBox_GetResult();
    if (iVar1 == 0) {
      WirelessLobbyChild_RequestRestart();
      WirelessLobbyChild_RequestCancel();
      VsMenu_Unk_ov001_022da33c(piVar2);
      VsMenu_Unk_ov001_022da850(piVar2);
      return;
    }
    VsMenu_Unk_ov001_022da33c(piVar2);
    *(undefined4 *)(*((int *)gTaskCurrent) + 8) = ((int)VsMenu_State_Unk_ov001_022db150);
  }
  return;
}

#pragma thumb reset
