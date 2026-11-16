#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022e9128;

typedef struct {
    s32 state;
    s32 unk4;
    u8 pad8[4];
    s32 unkc;
    s32 unk10;
    u8 pad14[0x190 - 0x14];
    UnkTask_022e9128 *task;
} UnkWork_022e9128;

typedef struct {
    u8 pad[0x10];
    UnkWork_022e9128 *work;
} UnkTask2_022e9128;

extern UnkTask2_022e9128 *gTaskCurrent;
extern u16 gDisplayMain[];

void ZoneMapPause_State_ConfirmFlash(void);

#pragma thumb on
void ZoneMapPause_Action_ConfirmB(UnkWork_022e9128 *w) {
    if (w == NULL) {
        w = gTaskCurrent->work;
    }
    w->state = 7;
    w->unk4 = 0;

    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    w->unkc = 0;
    w->task->func = ZoneMapPause_State_ConfirmFlash;
}
#pragma thumb reset
