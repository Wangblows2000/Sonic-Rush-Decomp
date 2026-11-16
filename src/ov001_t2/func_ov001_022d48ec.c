#include <nitro/types.h>

typedef struct {
    s32 state;
    s16 a;
    s16 b;
    s32 c;
    s32 d;
} UnkWork_022d48ec;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d48ec *work;
} UnkTask_022d48ec;

UnkTask_022d48ec *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void Task_DestroyRange(u16 lo, u16 hi);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov001_022d4814(void);

#pragma thumb on
void func_ov001_022d48ec(s32 a, s32 b, s32 c, s32 d) {
    UnkWork_022d48ec *w;

    Task_DestroyRange(0x2000, 0x2000);
    if (a < -16) a = -16;
    if (a > 16) a = 16;
    if (b < -16) b = -16;
    if (b > 16) b = 16;
    w = Task_Create(func_ov001_022d4814, 0, 0, 0x2000, 0, 0x10)->work;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, w, 0x10);
    }
    w->state = 0;
    w->a = a;
    w->b = b;
    w->c = c;
    w->d = d;
}
#pragma thumb reset
