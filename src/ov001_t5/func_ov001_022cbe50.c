#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSprite_022cbe50;

typedef struct {
    u8 pad0[0x3bc];
    UnkSprite_022cbe50 spr;
    u32 anim;
} UnkStruct_022cbe50;

void AnimatorSprite_SetAnimation(void *s, u16 id);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);
void AnimatorSprite_Draw(void *s);

#pragma thumb on
void func_ov001_022cbe50(UnkStruct_022cbe50 *p, s16 x, s16 y) {
    UnkSprite_022cbe50 *s;
    u32 *a;

    s = &p->spr;
    a = &p->anim;
    if (*a != 0) {
        AnimatorSprite_SetAnimation(s, 0);
        *a = 0;
    }
    s->x = x;
    s->y = y;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);
}
#pragma thumb reset
