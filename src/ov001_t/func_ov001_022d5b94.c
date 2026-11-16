#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSpr_022d5b94;

void AnimatorSprite_SetAnimation(UnkSpr_022d5b94 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d5b94 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d5b94 *s);

#pragma thumb on
void func_ov001_022d5b94(u8 *p, s16 x) {
    UnkSpr_022d5b94 *spr = (UnkSpr_022d5b94 *)(p + 0x29c);
    s32 *cur = (s32 *)(p + 0x3bc);

    if (*cur != 0) {
        *cur = 0;
        AnimatorSprite_SetAnimation(spr, *cur);
    }
    spr->x = x;
    spr->y = 0;
    spr->flags &= ~4;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}
#pragma thumb reset
