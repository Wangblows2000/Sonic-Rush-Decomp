#include <nitro/types.h>

typedef struct {
    s32 state;
    s16 a;
    s16 b;
    s32 c;
    s32 d;
} UnkWork_022c8aa0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022c8aa0 *work;
} UnkTask_022c8aa0;

UnkTask_022c8aa0 *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void OptionsFadeTask_Main(void);

#pragma thumb on
void Options_CreateFadeTask(s32 a, s32 b, s32 c, s32 d) {
    UnkWork_022c8aa0 *w;

    if (a < -16) a = -16;
    if (a > 16) a = 16;
    if (b < -16) b = -16;
    if (b > 16) b = 16;
    w = Task_Create(OptionsFadeTask_Main, 0, 0, 0x3000, 0x1000, 0x10)->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, 0x10);
    }
    w->state = 0;
    w->a = a;
    w->b = b;
    w->c = c;
    w->d = d;
}
#pragma thumb reset
