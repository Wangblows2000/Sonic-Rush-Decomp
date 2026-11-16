#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSpr_022ca2d4;

typedef struct {
    u8 pad0[0xc0];
    UnkSpr_022ca2d4 spr;
    u8 pad[0x394 - 0xc0 - sizeof(UnkSpr_022ca2d4)];
    u32 anim;
} UnkWork_022ca2d4;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022ca2d4 *work;
} UnkStruct_022ca2d4;

void AnimatorSprite_SetAnimation(UnkSpr_022ca2d4 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ca2d4 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ca2d4 *s);

#pragma thumb on
void OptionsMenu_Unk_ov001_022ca2d4(UnkStruct_022ca2d4 *p, s32 x, s32 y, s32 mode) {
    UnkWork_022ca2d4 *w;
    UnkSpr_022ca2d4 *s;
    u16 anim;

    if (x < 0x100) {
        w = p->work;
        s = &w->spr;
        anim = 0;
        switch (mode) {
        case 0:
            break;
        case 1:
            anim += 1;
            break;
        case 2:
            anim += 2;
            break;
        }
        if (w->anim != anim) {
            AnimatorSprite_SetAnimation(s, anim);
            w->anim = anim;
        }
        x += 0xd8;
        s->x = x;
        y += 0x90;
        s->y = y;
        s->flags &= 0x60000;
        s->flags |= 0x20000;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
