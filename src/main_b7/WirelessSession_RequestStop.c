#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkWork_020508a8;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_020508a8 *work;
} UnkTask_020508a8;

extern UnkTask_020508a8 *data_022c4638;
extern UnkTask_020508a8 *data_022c4634;
UnkTask_020508a8 *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void WirelessSessionStop_Main(void);

#pragma thumb on
void WirelessSession_RequestStop(s32 a, s32 b) {
    u32 v = (a + 1) | ((b + 1) << 8);
    if (data_022c4638 == NULL) {
        data_022c4634 = Task_Create(WirelessSessionStop_Main, 0, 0, 0x1000, 0, 4);
        data_022c4634->work->unk0 = v;
    } else {
        data_022c4638->work->unk8 = v;
        data_022c4638->fn = WirelessSessionStop_Main;
    }
}
#pragma thumb reset
