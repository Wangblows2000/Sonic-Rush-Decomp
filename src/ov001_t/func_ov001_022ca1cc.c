#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022ca1cc;

typedef struct {
    u8 pad0[0x584];
    UnkSpr_022ca1cc spr[8];
    s32 anim[8];
} UnkWork_022ca1cc;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x3ac - 0x14];
    UnkWork_022ca1cc *work;
} UnkStruct_022ca1cc;

void AnimatorSprite_SetAnimation(UnkSpr_022ca1cc *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ca1cc *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ca1cc *s);

#pragma thumb on
void func_ov001_022ca1cc(UnkStruct_022ca1cc *p, s16 x, s16 y, s32 mode) {
    UnkWork_022ca1cc *w;
    UnkSpr_022ca1cc *spr;
    u16 a;

    if (p->unk10 == 0) return;
    w = p->work;
    spr = &w->spr[1];
    a = 0;
    switch (mode) {
    case 0:
        break;
    case 1:
        a += 1;
        break;
    case 2:
        a += 2;
        break;
    }
    if (w->anim[1] != a) {
        AnimatorSprite_SetAnimation(spr, a);
        w->anim[1] = a;
    }
    spr->x = x;
    spr->y = y;
    spr->unk46 = 2;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}
#pragma thumb reset
