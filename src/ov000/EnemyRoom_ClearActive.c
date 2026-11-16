#include <nitro/types.h>

typedef struct UnkStruct_0234232c {
    u8 pad[0x10c];
    u8 unk10C;
} UnkStruct_0234232c;

extern UnkStruct_0234232c gCamera;

void EnemyRoom_ClearActive(u8 v) {
    if (gCamera.unk10C == v)
        gCamera.unk10C = 0xff;
}
