#include <nitro/types.h>

extern u32 gRandSeed;

u8 *ExtraBossPlayer_CreateFxObj(s32 a, void *b, s32 c, s32 d, s32 e);
void *ExtraBoss_GetPlayer(void);
void ExtraBoss_InitCollider(void *a, void *b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
void ExtraBossObj_State_DestroyWhenOutOfArena(void);
void ExtraBoss_Meteor_OnHit(void);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void ExtraBoss_SpawnMeteorOfKind(void *p, s32 x, s32 y, s32 z, s32 kind) {
    u8 *t = ExtraBossPlayer_CreateFxObj(kind, p, x, y, z);
    if (t == NULL)
        return;
    *(void **)(t + 0x80) = ExtraBoss_GetPlayer();
    *(u16 *)(t + 0x98) = 0xffff;
    *(void (**)(void))(t + 0x74) = ExtraBossObj_State_DestroyWhenOutOfArena;
    *(s32 *)(t + 0x50) = -0x3000;
    *(s16 *)(t + 0x68) = (Rand() & 0x1ff) - 0xff;
    *(s16 *)(t + 0x6a) = (Rand() & 0x1ff) - 0xff;
    *(s16 *)(t + 0x6c) = (Rand() & 0x1ff) - 0xff;
    ExtraBoss_InitCollider(t + 0xb0, t, 2, 0, 0xa000, 0xa000, 0xa000, 0, 0, 0, 0);
    *(void (**)(void))(t + 0xb4) = ExtraBoss_Meteor_OnHit;
    *(u32 *)(t + 0xb8) |= 0x2000;
    *(u8 **)(t + 0x9c) = t + 0xb0;
    ExtraBoss_InitCollider(t + 0xe8, t, 1, 1, 0x9000, 0x9000, 0x9000, 0, 0, 0, 0);
    *(u32 *)(t + 0xf0) |= 0x2000;
    *(u8 **)(t + 0xa0) = t + 0xe8;
}
