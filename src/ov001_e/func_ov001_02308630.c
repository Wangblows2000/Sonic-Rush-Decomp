#include <nitro/types.h>

typedef struct UnkStruct_02308630_o {
    u8 pad0[0x48];
} UnkStruct_02308630_o;

typedef struct UnkStruct_02308630 {
    u8 pad0[0x270];
    UnkStruct_02308630_o a;
    UnkStruct_02308630_o b;
    UnkStruct_02308630_o c;
    u8 pad348[0xae4 - 0x348];
    s32 unkAE4;
} UnkStruct_02308630;

extern UnkStruct_02308630 *gSpecialStageHud;

void AnimatorSprite_SetAnimation(UnkStruct_02308630_o *s, u16 index);

void func_ov001_02308630(void) {
    UnkStruct_02308630 *g = gSpecialStageHud;
    s32 n = g->unkAE4;

    AnimatorSprite_SetAnimation(&g->a, n / 100 % 10 + 0x1e);
    AnimatorSprite_SetAnimation(&g->b, n / 10 % 10 + 0x1e);
    AnimatorSprite_SetAnimation(&g->c, n % 10 + 0x1e);
}
