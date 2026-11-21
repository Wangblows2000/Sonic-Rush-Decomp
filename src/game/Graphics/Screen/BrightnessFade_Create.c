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

extern u32 gSystemFlags;
extern UnkTask_02043c7c *gBrightnessFadeTask;

UnkTask_02043c7c *Task_Create(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void BrightnessFade_Apply(UnkWork_02043c7c *w);
void BrightnessFade_Main(void);
void BrightnessFade_Destructor(void);

UnkWork_02043c7c *BrightnessFade_Create(u16 flags, u16 v) {
    UnkTask_02043c7c *t;
    UnkWork_02043c7c *w;
    s32 grp;
    BOOL exists;

    grp = 0;
    exists = FALSE;
    if (gSystemFlags & 8)
        grp = 7;
    t = gBrightnessFadeTask;
    if (t != NULL) {
        w = t->work;
        exists = TRUE;
        t->unk16 |= grp;
    } else {
        vu16 z;
        t = Task_Create(BrightnessFade_Main, BrightnessFade_Destructor, grp, 0x3e00, 3, 8);
        if (t == (UnkTask_02043c7c *)OS_GetArenaLo(0))
            return NULL;
        gBrightnessFadeTask = t;
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
        BrightnessFade_Apply(w);
    return w;
}
