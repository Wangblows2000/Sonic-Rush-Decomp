#include <nitro/types.h>

extern s32 gEnemyRoomTable[];

BOOL EnemyRoom_Unregister(s32 unused, s32 i) {
    gEnemyRoomTable[i] = 0;
    return TRUE;
}
