#include <nitro/types.h>

typedef struct UnkStruct_02329110 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad2[4];
    u16 unk5C;
} UnkStruct_02329110;

void EffWater3D_State_DestroyOnAnimEnd(UnkStruct_02329110 *s) {
    if (s->unk5C & 8)
        s->unk54 |= 4;
}
