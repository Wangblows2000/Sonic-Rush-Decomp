#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022e91cc;

typedef struct {
    s32 state;
    s32 unk4;
    u8 pad8[4];
    s32 unkc;
    s32 unk10;
    u8 pad14[0x190 - 0x14];
    UnkTask_022e91cc *task;
} UnkWork_022e91cc;

typedef struct {
    u8 pad[0x10];
    UnkWork_022e91cc *work;
} UnkTask2_022e91cc;

extern UnkTask2_022e91cc *gTaskCurrent;
extern u16 gDisplayMain[];

void ZoneMapPause_State_Close(void);

#pragma thumb on
void ZoneMapPause_Action_CloseA(UnkWork_022e91cc *w) {
    if (w == NULL) {
        w = gTaskCurrent->work;
    }
    w->state = 4;
    w->unk4 = 0;

    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    w->unkc = 0;
    w->task->func = ZoneMapPause_State_Close;
}
#pragma thumb reset
