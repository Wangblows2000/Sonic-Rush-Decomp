#include <nitro/types.h>

typedef struct {
    s32 step;
} UnkWork_022d6470;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d6470 *work;
} UnkTask_022d6470;

extern UnkTask_022d6470 *gTaskCurrent;

void func_ov001_022d5c30(void);

#pragma thumb on
void func_ov001_022d6470(UnkTask_022d6470 *t) {
    if (t == NULL) {
        t = gTaskCurrent;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x300;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = func_ov001_022d5c30;
}
#pragma thumb reset
