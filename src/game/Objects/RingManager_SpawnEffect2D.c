#include <nitro/types.h>

typedef struct UnkStruct_0235a9b4 {
    u8 pad[0x24];
    s32 unk24;
    u8 pad1[0x198 - 0x28];
    s32 unk198;
    s32 unk19C;
} UnkStruct_0235a9b4;

void RingEffect2D_Create(s32 a, s32 b, s32 c, s32 d, s32 e);

void RingManager_SpawnEffect2D(UnkStruct_0235a9b4 *s, s32 *v) {
    RingEffect2D_Create(v[0], v[1], s->unk24, s->unk198, s->unk19C);
}
