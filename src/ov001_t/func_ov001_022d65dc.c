#include <nitro/types.h>

typedef struct {
    s32 step;
} UnkWork_022d65dc;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d65dc *work;
} UnkTask_022d65dc;

extern UnkTask_022d65dc *data_022b4574;

void func_ov001_022d61c0(void);

#pragma thumb on
void func_ov001_022d65dc(UnkTask_022d65dc *t) {
    if (t == NULL) {
        t = data_022b4574;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x200;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = func_ov001_022d61c0;
}
#pragma thumb reset
