#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*func)(void);
} UnkTask_022e90f4;

typedef struct {
    s32 state;
    s32 unk4;
    u8 pad8[4];
    s32 unkc;
    s32 unk10;
    u8 pad14[0x190 - 0x14];
    UnkTask_022e90f4 *task;
} UnkWork_022e90f4;

typedef struct {
    u8 pad[0x10];
    UnkWork_022e90f4 *work;
} UnkTask2_022e90f4;

extern UnkTask2_022e90f4 *data_022b4574;
extern u16 data_0208922c[];

void func_ov001_022e8f6c(void);

#pragma thumb on
void func_ov001_022e90f4(UnkWork_022e90f4 *w) {
    if (w == NULL) {
        w = data_022b4574->work;
    }
    w->state = 8;
    w->unk4 = 0;

    data_0208922c[4] = 0;
    data_0208922c[5] = 0;
    w->unkc = 0;
    w->task->func = func_ov001_022e8f6c;
}
#pragma thumb reset
