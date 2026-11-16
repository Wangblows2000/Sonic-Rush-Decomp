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

void func_02033550(UnkSpr_022c9d58 *s, u16 anim);
void func_0203344c(UnkSpr_022c9d58 *s, u32 a, u32 b);
void func_02032d2c(UnkSpr_022c9d58 *s);
void func_02032480(UnkSpr_022c9d58 *s, fx32 sx, fx32 sy, s32 a);

#define DRAW(spr)                                              \
    if (scaled && (sx != 0x1000 || sy != 0x1000)) {            \
        func_02032480(spr, sx, sy, 0);                         \
    } else {                                                   \
        func_02032d2c(spr);                                    \
    }

#pragma thumb on
void func_ov001_022c9d58(UnkStruct_022c9d58 *p, s32 x, s32 y, fx32 sx, fx32 sy) {
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
    spr = &w->spr[2];
    flags = &spr->flags;
    if (w->anim[2] != 8) {
        func_02033550(spr, 8);
        w->anim[2] = 8;
    }
    spr->x = x;
    spr->y = y;
    *flags |= 0x200;
    func_0203344c(spr, 0, 0);
    func_02032d2c(spr);

    spr = &w->spr[0];
    flags = &spr->flags;
    if (w->anim[0] != 6) {
        func_02033550(spr, 6);
        w->anim[0] = 6;
    }
    xp1 = x + 1;
    spr->x = xp1;
    yp1 = y + 1;
    spr->y = yp1;
    *flags &= ~0x80;
    *flags &= ~0x100;
    *flags |= 0x200;
    func_0203344c(spr, 0, 0);
    DRAW(spr)

    spr->x = xp1;
    ym1 = y - 1;
    spr->y = ym1;
    *flags &= ~0x80;
    *flags |= 0x100;
    *flags |= 0x200;
    func_0203344c(spr, 0, 0);
    DRAW(spr)

    xm1 = x - 1;
    spr->x = xm1;
    spr->y = ym1;
    *flags |= 0x80;
    *flags |= 0x100;
    *flags |= 0x200;
    func_0203344c(spr, 0, 0);
    DRAW(spr)

    spr->x = xm1;
    spr->y = yp1;
    *flags |= 0x80;
    *flags &= ~0x100;
    *flags |= 0x200;
    func_0203344c(spr, 0, 0);
    DRAW(spr)
}
#pragma thumb reset
