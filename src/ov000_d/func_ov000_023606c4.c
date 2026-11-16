#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2C - 8];
    u32 unk2C;
} UnkSpr_023606c4;

typedef struct {
    u8 pad0[0x660];
    UnkSpr_023606c4 spr;
} UnkStruct_023606c4;

void AnimatorSprite_ProcessAnimation(UnkSpr_023606c4 *spr, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_023606c4 *spr);
void AnimatorSprite_DrawRotScale(UnkSpr_023606c4 *spr, s32 sx, s32 sy, s32 c);

#pragma thumb on

void func_ov000_023606c4(UnkStruct_023606c4 *w, s16 x, s16 y, s32 scale) {
    UnkSpr_023606c4 *spr = &w->spr;

    spr->x = x;
    spr->y = y;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    if (scale == 0x1000) {
        spr->unk2C &= ~0x200;
        AnimatorSprite_Draw(spr);
    } else {
        spr->unk2C |= 0x200;
        AnimatorSprite_DrawRotScale(spr, scale, scale, 0);
    }
}

#pragma thumb reset
