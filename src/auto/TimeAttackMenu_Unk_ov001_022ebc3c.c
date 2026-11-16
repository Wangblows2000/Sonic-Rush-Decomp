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
int TimeAttackMenu_Unk_ov001_022eb294();
int TimeAttackMenu_IndexToGrid();
int TimeAttackMenu_Unk_ov001_022ea850();
int TimeAttackMenu_Unk_ov001_022ea5a8();
int TimeAttackMenu_Unk_ov001_022ea504();
int TimeAttackMenu_Unk_ov001_022ea41c();
int TimeAttackMenu_StopHdma();

#pragma thumb on

void TimeAttackMenu_Unk_ov001_022ebc3c(int param_1)

{
  undefined4 *puVar1;
  undefined4 local_18;
  undefined4 local_14 [2];
  
  if (param_1 == 0) {
    param_1 = *((int *)gTaskCurrent);
  }
  puVar1 = *(undefined4 **)(param_1 + 0x10);
  *puVar1 = 0;
  TimeAttackMenu_IndexToGrid(puVar1,puVar1[0x34],&local_18,local_14);
  TimeAttackMenu_Unk_ov001_022ea850(puVar1);
  TimeAttackMenu_Unk_ov001_022ea5a8(puVar1,local_18);
  TimeAttackMenu_Unk_ov001_022ea504(puVar1,local_18,local_14[0]);
  TimeAttackMenu_Unk_ov001_022ea41c(puVar1);
  TimeAttackMenu_StopHdma(puVar1);
  *(undefined4 *)(param_1 + 8) = ((int)TimeAttackMenu_Unk_ov001_022eb294);
  return;
}

#pragma thumb reset
