#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 from;
    s32 to;
    s32 delay;
    s32 duration;
} UnkWork_02051e0c;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02051e0c *work;
} UnkTask_02051e0c;

typedef struct {
    u8 pad0[0x10];
    UnkTask_02051e0c *task;
} UnkState_02051e0c;

extern UnkState_02051e0c *gSysSound;
void Task_Destroy(UnkTask_02051e0c *t);
UnkTask_02051e0c *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, u16 c, u16 d, s32 e);
void SysSoundBGMFade_Main(void);
void SysSoundBGMFade_Destructor(void);
void SysSound_SetBGMVolume(u8 v);

#pragma thumb on
void SysSound_FadeBGMVolume(s32 from, s32 to, s32 delay, s32 duration, u16 prio, u16 id) {
    UnkWork_02051e0c *w;
    if (gSysSound == NULL)
        return;
    if (gSysSound->task != NULL) {
        Task_Destroy(gSysSound->task);
        gSysSound->task = NULL;
    }
    gSysSound->task = Task_Create(SysSoundBGMFade_Main, SysSoundBGMFade_Destructor, 0, prio, id, 0x14);
    w = gSysSound->task->work;
    w->timer = 0;
    w->from = from;
    w->to = to;
    w->delay = delay;
    w->duration = duration;
    SysSound_SetBGMVolume(from);
}
#pragma thumb reset
