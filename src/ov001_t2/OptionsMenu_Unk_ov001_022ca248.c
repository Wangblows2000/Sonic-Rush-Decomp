#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x3e];
    u16 unk46;
} UnkSpr_022ca248;

typedef struct {
    u8 pad0[0x450];
    UnkSpr_022ca248 spr;
    u8 pad498[0x574 - 0x498];
    u32 anim;
} UnkWork_022ca248;

typedef struct {
    u8 pad0[0x10];
    s32 active;
    u8 pad14[0x3ac - 0x14];
    UnkWork_022ca248 *work;
} UnkStruct_022ca248;

void AnimatorSprite_SetAnimation(UnkSpr_022ca248 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ca248 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ca248 *s);

#pragma thumb on
void OptionsMenu_Unk_ov001_022ca248(UnkStruct_022ca248 *p, s32 x, s32 y, u32 mode) {
    UnkWork_022ca248 *w;
    UnkSpr_022ca248 *s;
    u16 anim;

    if (p->active != 0) {
        w = p->work;
        s = &w->spr;
        anim = 5;
        switch (mode) {
        case 0:
            break;
        case 1:
            anim += 1;
            break;
        case 2:
            anim += 2;
            break;
        case 3:
            anim += 3;
            break;
        }
        if (w->anim != anim) {
            AnimatorSprite_SetAnimation(s, anim);
            w->anim = anim;
        }
        s->x = x;
        s->y = y;
        s->unk46 = 2;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
