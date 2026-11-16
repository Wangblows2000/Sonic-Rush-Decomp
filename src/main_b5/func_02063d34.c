#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 cnt;
    s16 hi;
    s16 lo;
    s32 cur;
    s32 target;
    s32 step;
} UnkWork_02063d34;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_02063d34 *work;
} UnkTask_02063d34;

extern UnkTask_02063d34 *gTaskCurrent;
extern u16 data_022c4db8;

s32 func_02002094(s32 a, s32 b);
s32 func_0200203c(s32 a, s32 b);
void func_02063cb4(UnkWork_02063d34 *w);
void func_02063df4(void);

void func_02063d34(void) {
    UnkWork_02063d34 *w = gTaskCurrent->work;

    w->cnt++;
    if (w->cnt >= 2) {
        w->cur += w->step;
        w->cnt = 0;
    }
    if (w->cur == w->target) {
        gTaskCurrent->fn = func_02063df4;
        w->cur = w->target;
        data_022c4db8 = 0;
    }
    w->hi = func_02002094(w->cur, 10);
    if (w->hi == 0)
        w->hi = -1;
    w->lo = func_0200203c(w->cur, 10);
    func_02063cb4(w);
}
