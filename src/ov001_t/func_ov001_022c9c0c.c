#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022c9c0c;

typedef struct {
    u8 pad0[0x94c];
    UnkSpr_022c9c0c spr[8];
    u8 pad[0xbe8 - 0x94c - 8 * 0x48];
    s32 anim[8];
} UnkWork_022c9c0c;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022c9c0c *work;
} UnkStruct_022c9c0c;

void AnimatorSprite_SetAnimation(UnkSpr_022c9c0c *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022c9c0c *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022c9c0c *s);

#pragma thumb on
void func_ov001_022c9c0c(UnkStruct_022c9c0c *p, s32 x, s32 y, s32 n) {
    s32 t;
    s16 xs0;
    s16 xs2;
    s16 xs1;
    UnkWork_022c9c0c *w;
    UnkSpr_022c9c0c *spr;
    s32 h;
    s32 u;
    u16 a;

    w = p->work;
    h = (n + 1) / 100 % 10;
    xs0 = x;
    spr = &w->spr[5];
    if (h > 0) {
        a = h + 10;
    } else {
        a = 9;
    }
    if (w->anim[5] != a) {
        AnimatorSprite_SetAnimation(spr, a);
        w->anim[5] = a;
    }
    spr->x = xs0;
    spr->y = y;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);

    t = (n + 1) / 10 % 10;
    xs1 = x + 0x10;
    spr = &w->spr[4];
    if (h > 0 || t > 0) {
        a = t + 10;
    } else {
        a = 9;
    }
    if (w->anim[4] != a) {
        AnimatorSprite_SetAnimation(spr, a);
        w->anim[4] = a;
    }
    spr->x = xs1;
    spr->y = y;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);

    u = (n + 1) % 10;
    x += 0x20;
    xs2 = x;
    spr = &w->spr[3];
    if (h > 0 || t > 0 || u > 0) {
        a = u + 10;
    } else {
        a = 9;
    }
    if (w->anim[3] != a) {
        AnimatorSprite_SetAnimation(spr, a);
        w->anim[3] = a;
    }
    spr->x = xs2;
    spr->y = y;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
}
#pragma thumb reset
