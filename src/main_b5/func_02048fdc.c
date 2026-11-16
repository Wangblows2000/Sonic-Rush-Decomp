#include <nitro/types.h>

typedef struct {
    s32 count;
    s32 total;
    s16 from;
    s16 to;
    u32 flags;
    u16 *done;
} UnkWork_02048fdc;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02048fdc *work;
} UnkTask_02048fdc;

UnkTask_02048fdc *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02048eb4(void);

UnkTask_02048fdc *func_02048fdc(s32 total, s16 from, s16 to, u32 flags, u16 *done) {
    UnkTask_02048fdc *t;
    UnkWork_02048fdc *w;
    vu16 z;

    t = func_020372d0(func_02048eb4, NULL, 0, 0x4000, 0, 0x14);
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x14);
    w->total = total;
    w->from = from;
    w->to = to;
    w->flags = flags & 3;
    if (done != NULL) {
        *done = 0;
        w->done = done;
    }
    return t;
}
