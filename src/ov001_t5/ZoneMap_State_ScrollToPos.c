#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    u8 pad8[8];
} UnkPos_022e6fbc;

typedef struct {
    u8 pad0[0x674];
    UnkPos_022e6fbc pos[1];
} UnkSub_022e6fbc;

typedef struct {
    u8 pad0[0x4c];
    s32 target;
    s32 x;
    s32 y;
    s32 speed;
    s32 tx;
    s32 ty;
    u8 pad64[0x6c - 0x64];
    s32 dispX;
    s32 dispY;
    s32 timer;
    u8 pad78[0xa4 - 0x78];
    UnkSub_022e6fbc *sub;
} UnkWork_022e6fbc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022e6fbc *work;
} UnkTask_022e6fbc;

extern UnkTask_022e6fbc *gTaskCurrent;

fx32 FX_Sqrt(fx32 a);
fx32 FX_Div(fx32 a, fx32 b);
void ZoneMap_UpdateView(UnkWork_022e6fbc *w);
void ZoneMap_SelectNextState(void);

static inline fx32 FX_Mul_022e6fbc(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

#pragma thumb on
void ZoneMap_State_ScrollToPos(void) {
    UnkWork_022e6fbc *w;
    s32 x;
    s32 y;
    fx32 dx;
    fx32 dy;
    fx32 speed;
    fx32 len;
    BOOL done;

    w = gTaskCurrent->work;
    x = w->tx;
    y = w->ty;
    dx = (x << 12) - w->x;
    dy = (y << 12) - w->y;
    speed = w->speed;
    len = FX_Sqrt(FX_Mul_022e6fbc(dx, dx) + FX_Mul_022e6fbc(dy, dy));
    if (len != 0) {
        dx = FX_Div(FX_Mul_022e6fbc(dx, speed), len);
        dy = FX_Div(FX_Mul_022e6fbc(dy, speed), len);
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
        ZoneMap_SelectNextState();
    }
}
#pragma thumb reset
