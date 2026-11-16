#include <nitro/types.h>
#pragma opt_propagation off

typedef struct UnkStruct_02306244 {
    u8 pad0[8];
    void *task;
    u8 padC[0x5e0 - 0xc];
    s32 unk5E0;
} UnkStruct_02306244;

extern UnkStruct_02306244 *data_ov001_0231f5a8;
extern vu8 data_02087d14[];

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void ExtraBoss_Unk_ov001_023061b0(void);

void ExtraBoss_Unk_ov001_02306244(void) {
    vu8 *w = data_02087d14;

    data_ov001_0231f5a8->task = Task_Create(ExtraBoss_Unk_ov001_023061b0, 0, 0, 0x3100, 2, 0);
    data_ov001_0231f5a8->unk5E0 = 0x10;
    w[1] = 0;
    w[5] = 0x5a;
    w[0] = 0;
    w[4] = 0x5c;
    w[3] = 0xfe;
    w[7] = 0x5a;
    w[2] = 0;
    w[6] = 0x5c;
    w[8] = 2;
    w[9] = 2;
    w[10] = 1;
    *(vu32 *)(w + 0xc) |= 1;
}
