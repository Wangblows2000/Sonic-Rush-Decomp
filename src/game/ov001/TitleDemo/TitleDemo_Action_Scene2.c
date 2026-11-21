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

extern UnkTask_022d65dc *gTaskCurrent;

void TitleDemo_State_Scene2(void);

#pragma thumb on
void TitleDemo_Action_Scene2(UnkTask_022d65dc *t) {
    if (t == NULL) {
        t = gTaskCurrent;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x200;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = TitleDemo_State_Scene2;
}
#pragma thumb reset
