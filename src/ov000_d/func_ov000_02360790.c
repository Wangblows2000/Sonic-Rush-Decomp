#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_02360790;

typedef struct {
    u8 pad0[0x468];
    UnkSpr_02360790 spr[4];
} UnkStruct_02360790;

void AnimatorSprite_ProcessAnimation(UnkSpr_02360790 *spr, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_02360790 *spr);
void AnimatorSprite_DrawRotScale(UnkSpr_02360790 *spr, s32 sx, s32 sy, s32 c);

#pragma thumb on

void func_ov000_02360790(UnkStruct_02360790 *w, s32 x, s32 y, s32 scale) {
    UnkSpr_02360790 *s0;
    UnkSpr_02360790 *s2;
    UnkSpr_02360790 *s1;

    s0 = &w->spr[0];
    s0->x = x;
    s0->y = y;
    AnimatorSprite_ProcessAnimation(s0, 0, 0);
    if (scale == 0x1000) {
        AnimatorSprite_Draw(s0);
    } else {
        AnimatorSprite_DrawRotScale(s0, scale, scale, 0);
    }
    s1 = &w->spr[1];
    s1->x = x;
    s1->y = y;
    AnimatorSprite_ProcessAnimation(s1, 0, 0);
    if (scale == 0x1000) {
        AnimatorSprite_Draw(s1);
    } else {
        AnimatorSprite_DrawRotScale(s1, scale, scale, 0);
    }
    s2 = &w->spr[2];
    s2->x = x;
    s2->y = y;
    AnimatorSprite_ProcessAnimation(s2, 0, 0);
    if (scale == 0x1000) {
        AnimatorSprite_Draw(s2);
    } else {
        AnimatorSprite_DrawRotScale(s2, scale, scale, 0);
    }
    w = (UnkStruct_02360790 *)&w->spr[3];
    ((UnkSpr_02360790 *)w)->x = x;
    ((UnkSpr_02360790 *)w)->y = y;
    AnimatorSprite_ProcessAnimation((UnkSpr_02360790 *)w, 0, 0);
    if (scale == 0x1000) {
        AnimatorSprite_Draw((UnkSpr_02360790 *)w);
    } else {
        AnimatorSprite_DrawRotScale((UnkSpr_02360790 *)w, scale, scale, 0);
    }
}

#pragma thumb reset
