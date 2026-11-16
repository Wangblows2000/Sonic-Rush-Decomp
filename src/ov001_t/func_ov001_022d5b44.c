#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSpr_022d5b44;

void AnimatorSprite_SetAnimation(UnkSpr_022d5b44 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d5b44 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d5b44 *s);

#pragma thumb on
void func_ov001_022d5b44(u8 *p, s16 x) {
    UnkSpr_022d5b44 *spr = (UnkSpr_022d5b44 *)(p + 0x2e4);
    s32 *cur = (s32 *)(p + 0x3c0);

    if (*cur != 1) {
        *cur = 1;
        AnimatorSprite_SetAnimation(spr, *cur);
    }
    spr->x = x;
    spr->y = 0;
    spr->flags &= ~4;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}
#pragma thumb reset
