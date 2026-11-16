#include <nitro/types.h>

typedef struct UnkTask_022e95b8 UnkTask_022e95b8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0x190 - 0xc];
    UnkTask_022e95b8 *task;
} UnkWork_022e95b8;

struct UnkTask_022e95b8 {
    u8 pad0[0x10];
    UnkWork_022e95b8 *work;
};

extern UnkWork_022e95b8 *data_ov001_0231f060;

void func_ov001_022e9584(void);
void func_ov001_022e9510(void);
UnkTask_022e95b8 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov001_022e9408(UnkWork_022e95b8 *w);
void func_ov001_022e926c(UnkWork_022e95b8 *w);
void func_ov001_022e8d44(void);
void func_ov001_022e901c(void);
void func_ov001_022e8d5c(void);

#pragma thumb on
static inline u32 GetPlane_022e95b8(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022e95b8(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void func_ov001_022e95b8(BOOL flag) {
    UnkTask_022e95b8 *t;
    UnkWork_022e95b8 *w;

    func_ov001_022e9584();
    func_ov001_022e9510();
    t = Task_Create(func_ov001_022e901c, func_ov001_022e8d5c, 0, 0x8000, 0x1000, 0x194);
    w = t->work;
    data_ov001_0231f060 = w;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, w, 0x194);
    }
    data_ov001_0231f060->unk0 = 0;
    data_ov001_0231f060->unk4 = 0;
    data_ov001_0231f060->unk8 = 0;
    data_ov001_0231f060->task = t;
    func_ov001_022e9408(data_ov001_0231f060);
    func_ov001_022e926c(data_ov001_0231f060);
    SetPlane_022e95b8(GetPlane_022e95b8() | 4);
    if (flag) {
        func_ov001_022e8d44();
    }
}
#pragma thumb reset
