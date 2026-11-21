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
int ZoneActSelect_IsFinished();
int ZoneActSelect_GetSelectedZone();
int ZoneActSelect_GetResult();
int ZoneMap_RestoreGraphics();
int ZoneMap_SetCharModelRotY();
int ZoneMap_SetCharModelAnim();
int ZoneMap_CreateFadeTask();
int ZoneMap_Action_WaitTimer();

#pragma thumb on

void ZoneMap_State_Unk_ov001_022e736c(void)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  
  puVar3 = *(undefined4 **)(*((int *)gTaskCurrent) + 0x10);
  puVar3[0x1d] = puVar3[0x1d] + 1;
  iVar1 = ZoneActSelect_IsFinished();
  if (iVar1 != 0) {
    iVar1 = ZoneActSelect_GetSelectedZone();
    iVar2 = ZoneActSelect_GetResult();
    if ((iVar1 < 0) || (iVar2 < 0)) {
      ZoneMap_RestoreGraphics(puVar3[0x29]);
      ZoneMap_SetCharModelRotY(puVar3[0x29] + 0x1994,0x1000);
      ZoneMap_SetCharModelAnim(puVar3[0x29] + 0x1994,0);
      ZoneMap_CreateFadeTask(0x10,0,8,0,1,0);
      ZoneMap_Action_WaitTimer(8);
      return;
    }
    *(undefined4 *)(puVar3[0x29] + 0x1cc8) = *puVar3;
    *(int *)(puVar3[0x29] + 0x1ccc) = iVar1;
    *(int *)(puVar3[0x29] + 0x1cd0) = iVar2;
    *(undefined4 *)puVar3[0x29] = 1;
  }
  return;
}

#pragma thumb reset
