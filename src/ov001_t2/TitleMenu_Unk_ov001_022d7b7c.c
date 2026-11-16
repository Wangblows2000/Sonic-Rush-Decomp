#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
} UnkSpr_022d7b7c;

typedef struct {
    u8 pad0[0x25c];
    UnkSpr_022d7b7c spr;
    u8 pad[0x7b4 - 0x25c - sizeof(UnkSpr_022d7b7c)];
    s32 anim;
} UnkObj_022d7b7c;

typedef struct {
    u8 pad0[0x1a4];
    UnkObj_022d7b7c *obj;
} UnkWork_022d7b7c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d7b7c *work;
} UnkTask_022d7b7c;

extern UnkTask_022d7b7c *gTaskCurrent;

void AnimatorSprite_SetAnimation(UnkSpr_022d7b7c *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022d7b7c *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022d7b7c *s);

#pragma thumb on
void TitleMenu_Unk_ov001_022d7b7c(s32 x, s32 y, s32 mode) {
    UnkWork_022d7b7c *w = gTaskCurrent->work;
    UnkSpr_022d7b7c *s;
    s32 anim;

    switch (mode) {
    case 0:
        anim = 0;
        break;
    case 1:
        return;
    case 2:
        anim = 1;
        break;
    default:
        return;
    }
    s = &w->obj->spr;
    if (w->obj->anim != anim) {
        AnimatorSprite_SetAnimation(s, anim);
        w->obj->anim = anim;
    }
    s->x = x;
    s->y = y;
    s->flags &= ~4;
    s->flags &= ~1;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
