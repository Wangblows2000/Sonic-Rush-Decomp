#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022cb4d8;

typedef struct {
    u8 pad0[0xc];
    s32 unkc;
    u8 pad10[0x300 - 0x10];
    UnkSpr_022cb4d8 spr;
    u8 pad308[0x3b4 - 0x308];
    u32 anim;
} UnkWork_022cb4d8;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb4d8 *work;
} UnkStruct_022cb4d8;

void AnimatorSprite_SetAnimation(UnkSpr_022cb4d8 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022cb4d8 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022cb4d8 *s);

#pragma thumb on
void OptionsMenu_DrawItemLabel2(UnkStruct_022cb4d8 *p, s32 x, s32 y, BOOL alt) {
    UnkWork_022cb4d8 *w = p->work;
    UnkSpr_022cb4d8 *s = &w->spr;
    u16 anim;

    if (w->unkc != 0) {
        anim = 0x11;
    } else {
        anim = 0x13;
    }
    if (alt) {
        anim++;
    }
    if (w->anim != anim) {
        AnimatorSprite_SetAnimation(s, anim);
        w->anim = anim;
    }
    s->x = x - 0x44;
    s->y = y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
