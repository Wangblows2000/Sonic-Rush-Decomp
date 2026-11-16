#include <nitro/types.h>

typedef struct {
    u16 flags;
    u16 unk2;
    u16 unk4;
    u16 unk6;
} UnkWork_02043c7c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02043c7c *work;
    u8 pad14[2];
    u16 unk16;
} UnkTask_02043c7c;

extern u32 data_02087cb8;
extern UnkTask_02043c7c *data_022c4410;

UnkTask_02043c7c *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 func_02006abc(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_02043abc(UnkWork_02043c7c *w);
void func_02043b78(void);
void func_02043c68(void);

UnkWork_02043c7c *func_02043c7c(u16 flags, u16 v) {
    UnkTask_02043c7c *t;
    UnkWork_02043c7c *w;
    s32 grp;
    BOOL exists;

    grp = 0;
    exists = FALSE;
    if (data_02087cb8 & 8)
        grp = 7;
    t = data_022c4410;
    if (t != NULL) {
        w = t->work;
        exists = TRUE;
        t->unk16 |= grp;
    } else {
        vu16 z;
        t = func_020372d0(func_02043b78, func_02043c68, grp, 0x3e00, 3, 8);
        if (t == (UnkTask_02043c7c *)func_02006abc(0))
            return NULL;
        data_022c4410 = t;
        w = t->work;
        z = 0;
        MIi_CpuClear16(z, w, 8);
    }
    w->unk4 = v;
    w->flags = flags;
    w->unk6 = 0;
    if (!exists || (w->flags & 0x10)) {
        if (w->flags & 2)
            w->unk2 = 0x1100;
        else
            w->unk2 = 0;
    }
    if (!(w->flags & 8))
        func_02043abc(w);
    return w;
}
