#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
} UnkSprite_022d5af0;

typedef struct {
    u8 pad0[4];
    s32 timer;
    u8 pad8[0x374 - 8];
    UnkSprite_022d5af0 spr;
    u8 padx[0x3c8 - 0x374 - sizeof(UnkSprite_022d5af0)];
    u32 anim;
} UnkStruct_022d5af0;

void AnimatorSprite_SetAnimation(void *s, u16 id);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void AnimatorSprite_Draw(void *s);

#pragma thumb on
void TitleDemo_Unk_ov001_022d5af0(UnkStruct_022d5af0 *p) {
    UnkSprite_022d5af0 *s;
    u32 *a;

    s = &p->spr;
    a = &p->anim;
    if (*a != 1) {
        *a = 1;
        AnimatorSprite_SetAnimation(s, *a);
    }
    s->x = 0;
    s->y = 0;
    s->flags &= ~4;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
    p->timer = 0;
}
#pragma thumb reset
