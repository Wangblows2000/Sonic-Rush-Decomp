#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;

typedef struct {
    s32 unk0;
    u8 pad4[4];
} UnkWork_0231fc1c;

extern UnkStruct_022c4560 data_022c4560;
extern s32 data_ov000_023644e0[][3];

void func_ov000_0231fbf0(void);

void func_ov000_0231fc1c(void) {
    UnkStruct_Task *task;
    UnkWork_0231fc1c *w;
    vu16 zero;

    task = Task_Create(func_ov000_0231fbf0, NULL, 0, 0x3eff, 3, sizeof(UnkWork_0231fc1c));
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_0231fc1c));
    w->unk0 = data_ov000_023644e0[data_022c4560.unk1C][data_022c4560.unk1D];
}
