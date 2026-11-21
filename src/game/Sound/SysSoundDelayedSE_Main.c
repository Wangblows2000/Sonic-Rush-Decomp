#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 duration;
    s32 arg;
} UnkWork_020518dc;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_020518dc *work;
} UnkTask_020518dc;

extern s32 gSysSound;
extern UnkTask_020518dc *gTaskCurrent;
void Task_Destroy(UnkTask_020518dc *t);
void SysSound_PlaySE(s32 a);

#pragma thumb on
void SysSoundDelayedSE_Main(void) {
    UnkWork_020518dc *w;
    if (gSysSound == 0) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w = gTaskCurrent->work;
    w->timer++;
    if (w->timer >= w->duration) {
        SysSound_PlaySE(w->arg);
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
