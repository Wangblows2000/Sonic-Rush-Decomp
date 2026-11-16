#include <nitro/types.h>

typedef struct {
    s32 timer;
    s32 from;
    s32 to;
    s32 delay;
    s32 duration;
} UnkWork_02051950;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02051950 *work;
} UnkTask_02051950;

extern UnkTask_02051950 *gTaskCurrent;
void Task_Destroy(UnkTask_02051950 *t);
void SysSound_SetBGMVolume(u8 v);

#pragma thumb on
void SysSoundBGMFade_Main(void) {
    UnkWork_02051950 *w = gTaskCurrent->work;
    s32 t = w->timer - w->delay;
    if (t < 0) {
        w->timer++;
        return;
    }
    SysSound_SetBGMVolume(w->from + (t + 1) * (w->to - w->from) / w->duration);
    w->timer++;
    if (w->timer >= w->delay + w->duration)
        Task_Destroy(gTaskCurrent);
}
#pragma thumb reset
