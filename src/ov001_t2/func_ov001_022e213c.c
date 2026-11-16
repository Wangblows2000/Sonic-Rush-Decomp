#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
} UnkWork_022e213c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e213c *work;
} UnkTask_022e213c;

UnkTask_022e213c *func_020372d0(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_ov001_022e2054(void);

#pragma thumb on
void func_ov001_022e213c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    UnkWork_022e213c *w;

    if (a < -16) a = -16;
    if (a > 16) a = 16;
    if (b < -16) b = -16;
    if (b > 16) b = 16;
    w = func_020372d0(func_ov001_022e2054, 0, 0, 0x4000, 0x1000, sizeof(UnkWork_022e213c))->work;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, w, sizeof(UnkWork_022e213c));
    }
    w->state = 0;
    w->a = a;
    w->b = b;
    w->c = c;
    w->d = d;
    w->e = e;
    w->f = f;
}
#pragma thumb reset
