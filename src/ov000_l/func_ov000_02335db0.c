#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_02335db0;

typedef struct {
    void (*fn)(void);
    u8 pad4[0x18];
    UnkVec_02335db0 v;
} UnkWork_02335db0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02335db0 *work;
} UnkTask_02335db0;

extern UnkTask_02335db0 *data_ov000_0236aa90;
extern u16 data_ov000_0236aa78;
extern u16 data_ov000_0236aa7c;
extern u16 data_ov000_0236aa80;
extern UnkVec_02335db0 data_ov000_0236aa9c;

void func_ov000_02335554(void);
void func_ov000_023355ac(void);
void func_ov000_023354ac(void);
UnkTask_02335db0 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

void func_ov000_02335db0(void) {
    UnkTask_02335db0 *task;
    UnkWork_02335db0 *w;
    vu16 zero;

    task = Task_Create(func_ov000_02335554, func_ov000_023355ac, 0, 0x90, 3, sizeof(UnkWork_02335db0));
    w = task->work;
    data_ov000_0236aa90 = task;
    zero = 0;
    data_ov000_0236aa78 = 0x100;
    data_ov000_0236aa7c = 0x10;
    data_ov000_0236aa80 = 0x100;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_02335db0));
    w->v.x = data_ov000_0236aa9c.x;
    w->v.y = data_ov000_0236aa9c.y;
    w->v.z = data_ov000_0236aa9c.z;
    w->fn = func_ov000_023354ac;
}
