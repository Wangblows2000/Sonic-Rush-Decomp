#include <nitro/types.h>

extern s32 gEnemyRoomTable[];

s32 EnemyRoom_Get(s32 unused, s32 i) {
    return gEnemyRoomTable[i];
}
