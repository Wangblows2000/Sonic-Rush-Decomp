#include <nitro/types.h>

typedef struct UnkStruct_02342358 {
    u8 pad[0x10c];
    u8 unk10C;
} UnkStruct_02342358;

extern UnkStruct_02342358 gCamera;
extern s32 gEnemyRoomTable[];

s32 EnemyRoom_GetActive(void) {
    u8 i = gCamera.unk10C;
    if (i != 0xff)
        return gEnemyRoomTable[i];
    return 0;
}
