#include <nitro/types.h>

typedef struct UnkStruct_02307990_o {
    u8 pad0[0x48];
} UnkStruct_02307990_o;

typedef struct UnkStruct_02307990_p {
    s16 x;
    s16 y;
} UnkStruct_02307990_p;

typedef struct UnkStruct_02307990 {
    u8 pad0[0xbf8];
    s16 posX;
    s16 posY;
    UnkStruct_02307990_o objs3[3];
    s32 timers[3];
    UnkStruct_02307990_p pos[3];
} UnkStruct_02307990;

extern UnkStruct_02307990 *gSpecialStageHud;
extern u32 gRandSeed;

void AnimatorSprite_SetAnimation(UnkStruct_02307990_o *s, u16 index);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_02307990(s32 i) {
    UnkStruct_02307990 *g = gSpecialStageHud;

    g->timers[i] = Rand() % 10 + 15;
    g->pos[i].x = g->posX + (Rand() % 11 - 5);
    g->pos[i].y = g->posY + (Rand() % 9 - 15);
    AnimatorSprite_SetAnimation(&g->objs3[i], 0x40);
}
