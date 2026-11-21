#include <nitro/types.h>

typedef struct {
    s32 step;
} UnkWork_022d6630;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d6630 *work;
} UnkTask_022d6630;

extern UnkTask_022d6630 *gTaskCurrent;

void TitleDemo_State_Scene1(void);

#pragma thumb on
void TitleDemo_Action_Scene1(UnkTask_022d6630 *t) {
    if (t == NULL) {
        t = gTaskCurrent;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x200;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = TitleDemo_State_Scene1;
}
#pragma thumb reset
