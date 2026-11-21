#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
} UnkSprite_022d5a18;

typedef struct {
    u8 pad0[4];
    s32 timer;
    u8 pad8[0x374 - 8];
    UnkSprite_022d5a18 spr;
    u8 padx[0x3c8 - 0x374 - sizeof(UnkSprite_022d5a18)];
    u32 anim;
} UnkStruct_022d5a18;

void AnimatorSprite_SetAnimation(void *s, u16 id);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void AnimatorSprite_Draw(void *s);

#pragma thumb on
void TitleDemo_Unk_ov001_022d5a18(UnkStruct_022d5a18 *p) {
    UnkSprite_022d5a18 *s;
    u32 *a;
    u32 id;

    s = &p->spr;
    a = &p->anim;
    if (p->timer >= 0x10) {
        id = 1;
    } else {
        id = 2;
    }
    if (*a != id) {
        *a = id;
        AnimatorSprite_SetAnimation(s, *a);
        p->timer = 0;
    }
    s->x = 0;
    s->y = 0;
    s->flags &= ~4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    p->timer++;
}
#pragma thumb reset
