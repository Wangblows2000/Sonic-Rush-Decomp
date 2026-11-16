#include <nitro/types.h>

typedef struct {
    u32 t;
    s16 from;
    s16 to;
    u32 dur;
    u32 delay;
} UnkWork_022c89c8;

typedef struct {
    u8 pad[0x10];
    UnkWork_022c89c8 *work;
} UnkTask_022c89c8;

typedef struct {
    u8 pad[8];
    s16 bright;
} UnkBright_022c89c8;

extern UnkTask_022c89c8 *gTaskCurrent;
extern UnkBright_022c89c8 data_02087d44;
extern UnkBright_022c89c8 data_0208926c;

fx32 func_0200225c(fx32 a, fx32 b);
void Task_Destroy(UnkTask_022c89c8 *t);

#define FX_MUL(a, b) ((fx32)(((s64)(a) * (b) + 0x800) >> 12))

#pragma thumb on
void TryAgainFadeTask_Main(void) {
    UnkWork_022c89c8 *w = gTaskCurrent->work;
    fx32 a, b;
    s32 v;

    if (w->t < w->delay) {
        w->t++;
        data_02087d44.bright = w->from;
        data_0208926c.bright = data_02087d44.bright;
        return;
    }
    a = w->from << 12;
    b = w->to << 12;
    if (a >= 0) {
        a += 0x800;
    } else {
        a -= 0x800;
    }
    if (b >= 0) {
        b += 0x800;
    } else {
        b -= 0x800;
    }
    v = (a + func_0200225c(FX_MUL((fx32)((w->t - w->delay) << 12), b - a), w->dur << 12)) >> 12;
    if (v < -16) v = -16;
    if (v > 16) v = 16;
    data_02087d44.bright = v;
    data_0208926c.bright = data_02087d44.bright;
    w->t++;
    if (w->t - w->delay >= w->dur) {
        data_02087d44.bright = w->to;
        data_0208926c.bright = data_02087d44.bright;
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
