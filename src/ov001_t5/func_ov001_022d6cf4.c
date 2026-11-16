#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    s32 unk10;
    u8 pad14[0x48c - 0x14];
    void *task;
    u8 pad490[0x4a4 - 0x490];
} UnkWork_022d6cf4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d6cf4 *work;
} UnkTask_022d6cf4;

UnkTask_022d6cf4 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022d6378(void);
void func_ov001_022d682c(void);

#pragma thumb on
UnkWork_022d6cf4 *func_ov001_022d6cf4(void) {
    UnkTask_022d6cf4 *t;
    UnkWork_022d6cf4 *w;

    t = Task_Create(func_ov001_022d6378, func_ov001_022d682c, 0, 0x1000, 0, sizeof(UnkWork_022d6cf4));
    w = t->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, sizeof(UnkWork_022d6cf4));
    }
    w->unk0 = 0;
    w->unk4 = 0;
    w->task = t;
    w->unk8 = 0;
    w->unkc = 0;
    w->unk10 = 0;
    return w;
}
#pragma thumb reset
