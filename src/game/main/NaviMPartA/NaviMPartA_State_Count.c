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
extern u16 gNaviMCounterPending;

s32 FX_DivS32(s32 a, s32 b);
s32 FX_ModS32(s32 a, s32 b);
void NaviMPartA_Draw(UnkWork_02063d34 *w);
void NaviMPartA_Main(void);

void NaviMPartA_State_Count(void) {
    UnkWork_02063d34 *w = gTaskCurrent->work;

    w->cnt++;
    if (w->cnt >= 2) {
        w->cur += w->step;
        w->cnt = 0;
    }
    if (w->cur == w->target) {
        gTaskCurrent->fn = NaviMPartA_Main;
        w->cur = w->target;
        gNaviMCounterPending = 0;
    }
    w->hi = FX_DivS32(w->cur, 10);
    if (w->hi == 0)
        w->hi = -1;
    w->lo = FX_ModS32(w->cur, 10);
    NaviMPartA_Draw(w);
}
