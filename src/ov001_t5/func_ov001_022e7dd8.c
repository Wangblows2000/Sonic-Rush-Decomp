#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[8];
} UnkPos_022e7dd8;

typedef struct {
    u8 pad0[0x674];
    UnkPos_022e7dd8 pos[1];
} UnkSub_022e7dd8;

typedef struct {
    u8 pad0[0x4c];
    s32 target;
    s32 x;
    s32 y;
    s32 speed;
    u8 pad5c[0x6c - 0x5c];
    s32 dispX;
    s32 dispY;
    s32 timer;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e7dd8 *sub;
} UnkWork_022e7dd8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e7dd8 *work;
} UnkTask_022e7dd8;

extern UnkTask_022e7dd8 *data_022b4574;

void func_ov001_022e5164(void *a, s32 x, s32 y, s32 *ox, s32 *oy);
fx32 func_020021f0(fx32 a);
fx32 func_0200225c(fx32 a, fx32 b);
void func_ov001_022e6dec(UnkWork_022e7dd8 *w);
void func_ov001_022e7c50(void);

static inline fx32 FX_Mul_022e7dd8(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

#pragma thumb on
void func_ov001_022e7dd8(void) {
    UnkWork_022e7dd8 *w;
    UnkSub_022e7dd8 *s;
    UnkPos_022e7dd8 *p;
    s32 x;
    s32 y;
    fx32 dx;
    fx32 dy;
    fx32 speed;
    fx32 len;
    BOOL done;

    w = data_022b4574->work;
    s = w->sub;
    p = &s->pos[w->target];
    x = p->x;
    y = p->y;
    func_ov001_022e5164((u8 *)s + 4, x, y, &x, &y);
    dx = (x << 12) - w->x;
    dy = (y << 12) - w->y;
    speed = w->speed;
    len = func_020021f0(FX_Mul_022e7dd8(dx, dx) + FX_Mul_022e7dd8(dy, dy));
    if (len != 0) {
        dx = func_0200225c(FX_Mul_022e7dd8(dx, speed), len);
        dy = func_0200225c(FX_Mul_022e7dd8(dy, speed), len);
        w->x += dx;
        w->y += dy;
        w->dispX = w->x >> 12;
        w->dispY = w->y >> 12;
    } else {
        w->dispX = x;
        w->dispY = y;
        w->x = w->dispX << 12;
        w->y = w->dispY << 12;
    }
    done = TRUE;
    if (dx >= 0) {
        if (x <= w->dispX) {
            w->dispX = x;
        } else {
            done = FALSE;
        }
    } else {
        if (x >= w->dispX) {
            w->dispX = x;
        } else {
            done = FALSE;
        }
    }
    if (dy >= 0) {
        if (y <= w->dispY) {
            w->dispY = y;
        } else {
            done = FALSE;
        }
    } else {
        if (y >= w->dispY) {
            w->dispY = y;
        } else {
            done = FALSE;
        }
    }
    func_ov001_022e6dec(w);
    w->timer++;
    if (done) {
        w->timer = 0;
        data_022b4574->func = func_ov001_022e7c50;
    }
}
#pragma thumb reset
