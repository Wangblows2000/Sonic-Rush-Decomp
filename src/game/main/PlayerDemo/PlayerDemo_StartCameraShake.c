#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    void *ptr;
    u16 val;
} UnkWork_02057244;

typedef struct {
    u8 pad0[0x10];
    void *work;
} UnkTask_02057244;

typedef struct {
    u8 pad0[0x24];
    UnkTask_02057244 *child;
} UnkMgr_02057244;

extern UnkTask_02057244 *gPlayerDemoTask;

UnkTask_02057244 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void PlayerDemoCameraShake_Main(void);
void PlayerDemoCameraShake_Destructor(void);

#pragma thumb on
void PlayerDemo_StartCameraShake(void *p, u16 v) {
    UnkMgr_02057244 *m;
    UnkTask_02057244 *t;
    UnkWork_02057244 *w;
    vu16 z;

    if (p == NULL || gPlayerDemoTask == NULL)
        return;
    m = gPlayerDemoTask->work;
    if (m->child != NULL)
        return;
    t = Task_Create(PlayerDemoCameraShake_Main, PlayerDemoCameraShake_Destructor, 0, 0x3efe, 3, 0x14);
    if (t == (UnkTask_02057244 *)OS_GetArenaLo(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x14);
    m->child = t;
    w->val = v;
    w->ptr = p;
}
#pragma thumb reset
