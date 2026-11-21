#include <nitro/types.h>

extern s32 gEnemyRoomTable[];

BOOL EnemyRoom_Register(s32 unused, s32 v, s32 i) {
    gEnemyRoomTable[i] = v;
    return TRUE;
}
