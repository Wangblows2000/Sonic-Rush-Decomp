#include <nitro/types.h>

typedef struct UnkStruct_02342ef4 {
    u8 pad[0x3c0];
    u16 unk3c0;
} UnkStruct_02342ef4;

void EffVsConfusion_Create(UnkStruct_02342ef4 *s);

void Player_ApplyConfusion(UnkStruct_02342ef4 *s) {
    EffVsConfusion_Create(s);
    s->unk3c0 = 0x258;
}
