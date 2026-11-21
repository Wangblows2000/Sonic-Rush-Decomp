#include <nitro/types.h>

typedef struct UnkStruct_02342f14 {
    u8 pad[0x3c2];
    u16 unk3c2;
} UnkStruct_02342f14;

void EffVsSlowdown_Create(UnkStruct_02342f14 *s);

void Player_ApplySlowdown(UnkStruct_02342f14 *s) {
    EffVsSlowdown_Create(s);
    s->unk3c2 = 0x200;
}
