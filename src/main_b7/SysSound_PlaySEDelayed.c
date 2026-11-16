#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 duration;
    s32 arg;
    u32 unkc;
} UnkWork_02051db0;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02051db0 *work;
} UnkTask_02051db0;

extern void *gSysSound;
UnkTask_02051db0 *Task_Create(void (*fn)(void), void *dtor, s32 b, s32 c, s32 d, s32 e);
void SysSoundDelayedSE_Main(void);
void MIi_CpuClear16(u16 data, void *dst, u32 size);

#pragma thumb on
UnkTask_02051db0 *SysSound_PlaySEDelayed(s32 arg, s32 duration, s32 c, s32 d) {
    UnkTask_02051db0 *t;
    UnkWork_02051db0 *w;
    vu16 z;
    if (gSysSound == NULL)
        return NULL;
    t = Task_Create(SysSoundDelayedSE_Main, NULL, 0, c, d, 0x10);
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x10);
    w->timer = 0;
    w->duration = duration;
    w->arg = arg;
    return t;
}
#pragma thumb reset
