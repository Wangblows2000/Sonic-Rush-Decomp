#include <nitro/types.h>

typedef struct {
    void (*state)(void *);
    u32 pad4;
} UnkWork_02060288;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02060288 *work;
} UnkTask_02060288;

UnkTask_02060288 *Task_Create(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void TalkDemoDim_Main(void);
void TalkDemoDim_Destructor(void);
void TalkDemoDim_State_Init(void *t);

#pragma thumb on
void TalkDemoDim_Create(void) {
    UnkTask_02060288 *t;
    UnkWork_02060288 *w;
    vu16 z;

    t = Task_Create(TalkDemoDim_Main, TalkDemoDim_Destructor, 7, 0xeffe, 3, 8);
    if (t == (UnkTask_02060288 *)OS_GetArenaLo(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 8);
    w->state = TalkDemoDim_State_Init;
}
#pragma thumb reset
