#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022c9d58;

typedef struct {
    u8 pad0[0x94c];
    UnkSpr_022c9d58 spr[8];
    u8 pad[0xbe8 - 0x94c - 8 * 0x48];
    s32 anim[8];
} UnkWork_022c9d58;

typedef struct {
    u8 pad0[0x3ac];
    UnkWork_022c9d58 *work;
} UnkStruct_022c9d58;

void AnimatorSprite_SetAnimation(UnkSpr_022c9d58 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022c9d58 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022c9d58 *s);
void AnimatorSprite_DrawRotScale(UnkSpr_022c9d58 *s, fx32 sx, fx32 sy, s32 a);

#define DRAW(spr)                                              \
    if (scaled && (sx != 0x1000 || sy != 0x1000)) {            \
        AnimatorSprite_DrawRotScale(spr, sx, sy, 0);                         \
    } else {                                                   \
        AnimatorSprite_Draw(spr);                                    \
    }

#pragma thumb on
void func_ov001_022c9f54(UnkStruct_022c9d58 *p, s32 x, s32 y, fx32 sx, fx32 sy) {
    BOOL scaled;
    UnkWork_022c9d58 *w;
    UnkSpr_022c9d58 *spr;
    u32 *flags;
    s32 ym1;
    s32 xp1;
    s32 xm1;
    s32 yp1;

    if (sx == 0x1000 && sy == 0x1000) {
        scaled = FALSE;
    } else {
        scaled = TRUE;
    }
    w = p->work;
    spr = &w->spr[1];
    flags = &spr->flags;
    if (w->anim[1] != 7) {
        AnimatorSprite_SetAnimation(spr, 7);
        w->anim[1] = 7;
    }
    spr->x = x;
    spr->y = y;
    *flags |= 0x200;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);

    spr = &w->spr[0];
    flags = &spr->flags;
    if (w->anim[0] != 6) {
        AnimatorSprite_SetAnimation(spr, 6);
        w->anim[0] = 6;
    }
    xp1 = x + 1;
    spr->x = xp1;
    yp1 = y + 1;
    spr->y = yp1;
    *flags &= ~0x80;
    *flags &= ~0x100;
    *flags |= 0x200;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    DRAW(spr)

    spr->x = xp1;
    ym1 = y - 1;
    spr->y = ym1;
    *flags &= ~0x80;
    *flags |= 0x100;
    *flags |= 0x200;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    DRAW(spr)

    xm1 = x - 1;
    spr->x = xm1;
    spr->y = ym1;
    *flags |= 0x80;
    *flags |= 0x100;
    *flags |= 0x200;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    DRAW(spr)

    spr->x = xm1;
    spr->y = yp1;
    *flags |= 0x80;
    *flags &= ~0x100;
    *flags |= 0x200;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    DRAW(spr)
}
#pragma thumb reset
