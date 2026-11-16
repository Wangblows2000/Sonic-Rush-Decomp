#include <nitro/types.h>

typedef struct {
    s32 mode;
    u8 pad4[0x10 - 4];
    s32 unk10;
    u8 pad14[0x190 - 0x14];
    void *task;
} UnkWork_022e90b8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e90b8 *work;
} UnkTask_022e90b8;

extern UnkTask_022e90b8 *gTaskCurrent;
extern s32 data_ov001_0231c588;

void Task_Destroy(void *task);

#pragma thumb on
void ZoneMapPause_Unk_ov001_022e90b8(UnkWork_022e90b8 *w) {
    if (w == NULL) {
        w = gTaskCurrent->work;
    }
    if (w->mode == 8 && w->unk10 == 0) {
        data_ov001_0231c588 = 0;
    } else {
        data_ov001_0231c588 = 1;
    }
    Task_Destroy(w->task);
}
#pragma thumb reset
