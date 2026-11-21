#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022c9b78;


typedef struct {
    u8 pad0[0xafc];
    UnkSpr_022c9b78 spr0;
    UnkSpr_022c9b78 spr1;
    u8 padb8c[0xc00 - 0xb8c];
    s32 anim0;
    s32 anim1;
} UnkWork_022c9b78;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022c9b78 *work;
} UnkStruct_022c9b78;

void AnimatorSprite_SetAnimation(UnkSpr_022c9b78 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022c9b78 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022c9b78 *s);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c9b78(UnkStruct_022c9b78 *p, s16 x, s16 y) {
    UnkWork_022c9b78 *w = p->work;
    UnkSpr_022c9b78 *s = &w->spr0;

    if (w->anim0 != 0x14) {
        AnimatorSprite_SetAnimation(s, 0x14);
        w->anim0 = 0x14;
    }
    s->x = x + 0x4c;
    s->y = y + 0x3c;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    s = &w->spr1;
    if (w->anim1 != 0x14) {
        AnimatorSprite_SetAnimation(s, 0x14);
        w->anim1 = 0x14;
    }
    s->x = x + 0xb4;
    s->y = y + 0x3c;
    s->flags |= 0x80;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
