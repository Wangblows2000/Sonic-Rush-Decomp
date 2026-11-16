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

void TitleDemo_State_Scene6(void);

#pragma thumb on
void TitleDemo_Action_Scene6(UnkTask_022d6470 *t) {
    if (t == NULL) {
        t = gTaskCurrent;
    }
    t->work->step = 0;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x300;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1200;
    t->func = TitleDemo_State_Scene6;
}
#pragma thumb reset
