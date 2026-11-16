#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u16 anim;
    u8 pada[0x3e];
} UnkSpr_022e19d0;

typedef struct {
    u8 pad0[0x14];
    u8 bg[0x1020 - 0x14];
    UnkSpr_022e19d0 sprA[21];
    u8 pad1608[0x70];
    UnkSpr_022e19d0 sprB[3];
    UnkSpr_022e19d0 sprC[8];
} UnkStruct_022e19d0;

void Background_Draw(void *bg);
void AnimatorSprite_SetAnimation(UnkSpr_022e19d0 *s, u16 anim);

#pragma thumb on
void func_ov001_022e19d0(UnkStruct_022e19d0 *p) {
    s32 i;
    UnkSpr_022e19d0 *s;
    UnkSpr_022e19d0 *t;
    s32 j;

    Background_Draw(p->bg);
    i = 0;
    s = p->sprA;
    for (; i < 21; i++) {
        AnimatorSprite_SetAnimation(s, s->anim);
        s++;
    }
    j = 0;
    t = p->sprB;
    for (; j < 3; j++) {
        AnimatorSprite_SetAnimation(t, t->anim);
        t++;
    }
    j = 0;
    t = p->sprC;
    for (; j < 8; j++) {
        AnimatorSprite_SetAnimation(t, t->anim);
        t++;
    }
}
#pragma thumb reset
