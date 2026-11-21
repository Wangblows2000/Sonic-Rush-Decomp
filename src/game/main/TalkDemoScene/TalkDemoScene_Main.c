#include <nitro/types.h>

typedef struct UnkWork_02062bb0 {
    void (*fn)(struct UnkWork_02062bb0 *w);
    u8 pad4[4];
    s32 unk8;
} UnkWork_02062bb0;
typedef struct { u8 pad0[0x10]; UnkWork_02062bb0 *work; } UnkTask_02062bb0;
extern UnkTask_02062bb0 *gTaskCurrent;

BOOL TalkDemo_IsActive(void);
void Task_Destroy(UnkTask_02062bb0 *t);
u32 TalkDemo_GetFlags(s32 a);
void TalkDemo_ResetBg2Affine(s32 a);
void TalkDemo_ResetBg1Scroll(void *p);
void TalkDemoScene_UpdateSound(UnkWork_02062bb0 *w);

#pragma thumb on
void TalkDemoScene_Main(void) {
    UnkWork_02062bb0 *w = gTaskCurrent->work;

    if (!TalkDemo_IsActive()) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (TalkDemo_GetFlags(3) & 0x40000000) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->unk8++;
    if (TalkDemo_GetFlags(3) & 0x20000000) {
        TalkDemo_ResetBg2Affine(0);
        TalkDemo_ResetBg1Scroll(NULL);
        w->fn = NULL;
    }
    if (w->fn != NULL)
        w->fn(w);
    TalkDemoScene_UpdateSound(w);
    if (TalkDemo_GetFlags(3) & 0x80000000)
        Task_Destroy(gTaskCurrent);
}
#pragma thumb reset
