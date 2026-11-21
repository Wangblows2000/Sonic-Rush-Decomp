#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022d7c04;

typedef struct {
    u8 pad0[0x25c];
    UnkSpr_022d7c04 sprites[19];
    s32 ids[19];
} UnkSprWork_022d7c04;

typedef struct {
    u8 pad0[0x1a4];
    UnkSprWork_022d7c04 *sw;
} UnkWork_022d7c04;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d7c04 *work;
} UnkTask_022d7c04;

extern UnkTask_022d7c04 *gTaskCurrent;

void AnimatorSprite_SetAnimation(UnkSpr_022d7c04 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d7c04 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022d7c04 *s);

#pragma thumb on
void TitleMenu_DrawItemButton(s32 idx, s16 x, s16 y, s32 mode, u32 t) {
    s32 base;
    s32 anim;
    UnkWork_022d7c04 *w;
    UnkSpr_022d7c04 *s;

    base = 0;
    anim = 0;
    w = gTaskCurrent->work;
    switch (mode) {
    case 0:
        base = 0;
        break;
    case 1:
    case 3:
        base = 1;
        break;
    case 2:
        base = 2;
        break;
    }
    switch (idx) {
    case 9:
        anim = base + 7;
        break;
    case 10:
        anim = base + 10;
        break;
    case 11:
        anim = base + 13;
        break;
    case 12:
        anim = base + 16;
        break;
    case 13:
        anim = base + 19;
        break;
    case 14:
        anim = base + 22;
        break;
    case 15:
        anim = base + 25;
        break;
    case 16:
        anim = base + 28;
        break;
    case 17:
        anim = base + 31;
        break;
    case 18:
        anim = base + 34;
        break;
    }
    if (mode == 1 || mode == 2) {
        UnkSprWork_022d7c04 *sw = w->sw;
        s = &sw->sprites[1];
        if (t < 7 && mode != 2) {
            if (sw->ids[1] != 4 || t == 0) {
                AnimatorSprite_SetAnimation(s, 4);
                w->sw->ids[1] = 4;
            }
        } else if (sw->ids[1] != 3) {
            AnimatorSprite_SetAnimation(s, 3);
            w->sw->ids[1] = 3;
        }
        s->x = x;
        s->y = y;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        if (t >= 7 && mode != 2) {
            if ((t - 7) % 60 < 40) {
                AnimatorSprite_Draw(s);
            }
        } else {
            s->x = x;
            s->y = y;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
    }
    s = &w->sw->sprites[idx];
    if (anim != w->sw->ids[idx]) {
        AnimatorSprite_SetAnimation(s, anim);
        w->sw->ids[idx] = anim;
    }
    s->x = x;
    s->y = y;
    s->flags |= 4;
    s->flags &= ~1;
    s->flags |= 0x40;
    s->flags |= 0x20;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
