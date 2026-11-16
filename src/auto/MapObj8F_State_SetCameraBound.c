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

int Gimmick_GetRoomIndex();
int EnemyRoom_Get();
int MapObj8F_Action_StartCameraLock();

void MapObj8F_State_SetCameraBound(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = Gimmick_GetRoomIndex();
  iVar2 = EnemyRoom_Get(1,uVar1);
  if (iVar2 != 0) {
    if ((*(ushort *)(*(int *)(param_1 + 0x1e0) + 4) & 2) == 0) {
      *(undefined4 *)(iVar2 + 0x220) = *(undefined4 *)(param_1 + 0x20c);
      *(undefined4 *)(iVar2 + 0x224) = *(undefined4 *)(param_1 + 0x210);
    }
    else {
      *(undefined4 *)(iVar2 + 0x228) = *(undefined4 *)(param_1 + 0x20c);
      *(undefined4 *)(iVar2 + 0x22c) = *(undefined4 *)(param_1 + 0x210);
    }
  }
  if (iVar2 == 0) {
    return;
  }
  if ((*(uint *)(iVar2 + 0x20c) & 0x1000) == 0) {
    return;
  }
  MapObj8F_Action_StartCameraLock(param_1);
  return;
}
