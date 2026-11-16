#include <nitro/types.h>

typedef struct {
    s32 step;
} UnkWork_022d6588;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d6588 *work;
} UnkTask_022d6588;

extern UnkTask_022d6588 *data_022b4574;

void func_ov001_022d6000(void);

#pragma thumb on
void func_ov001_022d6588(UnkTask_022d6588 *t) {
    if (t == NULL) {
        t = data_022b4574;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x200;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = func_ov001_022d6000;
}
#pragma thumb reset
