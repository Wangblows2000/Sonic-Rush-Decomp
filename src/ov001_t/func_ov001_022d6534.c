#include <nitro/types.h>

typedef struct {
    s32 step;
} UnkWork_022d6534;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d6534 *work;
} UnkTask_022d6534;

extern UnkTask_022d6534 *gTaskCurrent;

void func_ov001_022d5e78(void);

#pragma thumb on
void func_ov001_022d6534(UnkTask_022d6534 *t) {
    if (t == NULL) {
        t = gTaskCurrent;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x300;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = func_ov001_022d5e78;
}
#pragma thumb reset
