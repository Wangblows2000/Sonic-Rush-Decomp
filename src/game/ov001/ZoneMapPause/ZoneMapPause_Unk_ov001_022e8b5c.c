#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022e8b5c;

typedef struct {
    s32 mode;
    u8 pad4[8];
    s32 unkc;
    u8 pad10[0x54 - 0x10];
    UnkSpr_022e8b5c spr;
    u8 pad5c[0x174 - 0x5c];
    s32 anim;
} UnkWork_022e8b5c;

void AnimatorSprite_SetAnimation(UnkSpr_022e8b5c *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022e8b5c *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022e8b5c *s);

#pragma thumb on
void ZoneMapPause_Unk_ov001_022e8b5c(UnkWork_022e8b5c *w) {
    s32 anim;
    s32 y;

    switch (w->mode) {
    case 1:
    case 2:
    case 3:
    case 4:
        anim = 0;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        anim = 1;
        break;
    case 0:
        return;
    default:
        return;
    }
    y = -w->unkc;
    if (y >= -16) {
        if (anim != w->anim) {
            AnimatorSprite_SetAnimation(&w->spr, anim);
            w->anim = anim;
        }
        w->spr.x = 0;
        w->spr.y = y;
        AnimatorSprite_ProcessAnimation(&w->spr, 0, 0);
        AnimatorSprite_Draw(&w->spr);
    }
}
#pragma thumb reset
