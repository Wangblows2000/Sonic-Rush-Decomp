#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
} UnkSpr_022cb540;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padc[0x2b8 - 0xc];
    UnkSpr_022cb540 spr;
    u8 pad2c0[0x3b0 - 0x2c0];
    u32 anim;
} UnkWork_022cb540;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022cb540 *work;
} UnkStruct_022cb540;

void AnimatorSprite_SetAnimation(UnkSpr_022cb540 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022cb540 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022cb540 *s);

#pragma thumb on
void OptionsMenu_DrawItemLabel1(UnkStruct_022cb540 *p, s32 x, s32 y, BOOL alt) {
    UnkWork_022cb540 *w = p->work;
    UnkSpr_022cb540 *s = &w->spr;
    u16 anim;

    switch (w->unk8) {
    case 0:
        anim = 0xd;
        break;
    case 1:
        anim = 0xf;
        break;
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
