#include <nitro/types.h>

typedef struct UnkStruct_02355da0_2 {
    u8 pad[0x1e0];
    s32 unk1E0;
} UnkStruct_02355da0_2;

typedef struct UnkStruct_02355da0 {
    u8 pad[0x3e4];
    s32 unk3E4;
} UnkStruct_02355da0;

void Player_SetSourceMapObject(UnkStruct_02355da0 *a, UnkStruct_02355da0_2 *b) {
    a->unk3E4 = b->unk1E0;
}
