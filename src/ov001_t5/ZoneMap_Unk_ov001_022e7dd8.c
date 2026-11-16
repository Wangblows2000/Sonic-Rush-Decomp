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

extern UnkTask_022e7dd8 *gTaskCurrent;

void ZoneMap_CalcClampedScroll(void *a, s32 x, s32 y, s32 *ox, s32 *oy);
fx32 FX_Sqrt(fx32 a);
fx32 FX_Div(fx32 a, fx32 b);
void ZoneMap_UpdateView(UnkWork_022e7dd8 *w);
void ZoneMap_State_MoveAlongPath(void);

static inline fx32 FX_Mul_022e7dd8(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

#pragma thumb on
void ZoneMap_Unk_ov001_022e7dd8(void) {
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

    w = gTaskCurrent->work;
    s = w->sub;
    p = &s->pos[w->target];
    x = p->x;
    y = p->y;
    ZoneMap_CalcClampedScroll((u8 *)s + 4, x, y, &x, &y);
    dx = (x << 12) - w->x;
    dy = (y << 12) - w->y;
    speed = w->speed;
    len = FX_Sqrt(FX_Mul_022e7dd8(dx, dx) + FX_Mul_022e7dd8(dy, dy));
    if (len != 0) {
        dx = FX_Div(FX_Mul_022e7dd8(dx, speed), len);
        dy = FX_Div(FX_Mul_022e7dd8(dy, speed), len);
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
    ZoneMap_UpdateView(w);
    w->timer++;
    if (done) {
        w->timer = 0;
        gTaskCurrent->func = ZoneMap_State_MoveAlongPath;
    }
}
#pragma thumb reset
