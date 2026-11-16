#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 from;
    s32 to;
    s32 duration;
    s32 delay;
    s32 sub;
    s32 main;
} UnkWork_022e2054;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e2054 *work;
} UnkTask_022e2054;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022e2054;

extern UnkTask_022e2054 *gTaskCurrent;
extern UnkBright_022e2054 data_02087d44;
extern UnkBright_022e2054 data_0208926c;

s32 func_0200225c(s32 n, s32 d);
void Task_Destroy(UnkTask_022e2054 *t);

#pragma thumb on
static inline fx32 FX_Mul_022e2054(fx32 a, fx32 b) {
    return (fx32)(((s64)a * b + 0x800) >> 12);
}

void ZoneMapFadeTask_Main(void) {
    UnkWork_022e2054 *w;
    fx32 a;
    fx32 b;
    s32 v;

    w = gTaskCurrent->work;
    if (w->timer < w->delay) {
        w->timer++;
        data_02087d44.bright = w->from;
        data_0208926c.bright = data_02087d44.bright;
        return;
    }
    a = w->from << 12;
    b = w->to << 12;
    if (a >= 0) a += 0x800; else a -= 0x800;
    if (b >= 0) b += 0x800; else b -= 0x800;
    v = (a + func_0200225c(FX_Mul_022e2054((w->timer - w->delay) << 12, b - a), w->duration << 12)) >> 12;
    if (v < -16) v = -16;
    if (v > 16) v = 16;
    if (w->sub != 0) data_0208926c.bright = v;
    if (w->main != 0) data_02087d44.bright = v;
    w->timer++;
    if (w->timer - w->delay >= w->duration) {
        if (w->sub != 0) data_0208926c.bright = w->to;
        if (w->main != 0) data_02087d44.bright = w->to;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
