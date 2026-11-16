#include <nitro/types.h>

typedef struct UnkStruct_023081d8 {
    u8 pad0[0x270];
    u8 a[0x48];
    u8 b[0x48];
    u8 c[0x48];
} UnkStruct_023081d8;

extern UnkStruct_023081d8 *gSpecialStageHud;

void AnimatorSprite_DrawRotScale(void *p, s32 x, s32 y, s32 z);

void func_ov001_023081d8(s32 t) {
    UnkStruct_023081d8 *g = gSpecialStageHud;
    s32 sa = 0x1000;
    s32 sb = 0x1000;
    s32 sc = 0x1000;

    if (t >= 0 && t <= 0x14) {
        sa += 1228.8f * (0x14 - t) / 20.0f;
    } else if (t > 0x14 && t <= 0x28) {
        sb += 1228.8f * (0x28 - t) / 20.0f;
    } else if (t > 0x28 && t <= 0x3c) {
        sc += 1228.8f * (0x3c - t) / 20.0f;
    }
    AnimatorSprite_DrawRotScale(g->a, sa, sa, 0);
    AnimatorSprite_DrawRotScale(g->b, sb, sb, 0);
    AnimatorSprite_DrawRotScale(g->c, sc, sc, 0);
}
