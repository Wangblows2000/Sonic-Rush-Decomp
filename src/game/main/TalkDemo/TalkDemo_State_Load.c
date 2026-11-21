#include <nitro/types.h>

typedef struct UnkWork_02060d00 {
    void (*fn)(struct UnkWork_02060d00 *w);
    u8 pad4[0x1c];
    s32 unk20;
    void *unk24;
} UnkWork_02060d00;
typedef struct { u8 pad0[4]; s32 unk4; } UnkInfo_02060d00;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02060d00;
extern s16 gTalkDemoId;
extern void *gTaskCurrent;
extern UnkBlend_02060d00 data_0208926c;

void *TalkDemo_LoadScript(s32 a, u8 b);
void Task_Destroy(void *t);
UnkInfo_02060d00 *TalkDemo_GetScriptPage(void *p, s32 a);
void TalkDemo_SetFlags(s32 a, s32 b);
void TalkDemoScene_Create(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void TalkDemoWindow_Create(void);
u32 TalkDemo_GetFlags(s32 a);
void TalkDemoDim_Create(void);
void TalkDemo_LoadSound(UnkWork_02060d00 *w);
void TalkDemo_PlayBGM(UnkWork_02060d00 *w);
void TalkDemo_State_WaitWindowOpen(UnkWork_02060d00 *w);

#pragma thumb on
void TalkDemo_State_Load(UnkWork_02060d00 *w) {
    vu32 zero;

    w->unk24 = TalkDemo_LoadScript(w->unk20, gTalkDemoId);
    if (w->unk24 == NULL) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (TalkDemo_GetScriptPage(w->unk24, 0)->unk4 == 3) {
        TalkDemo_SetFlags(0, 2);
        data_0208926c.unk8 = 0;
    }
    TalkDemoScene_Create();
    zero = 0;
    MIi_CpuClear32(zero, (void *)0x06002000, 0xa000);
    TalkDemoWindow_Create();
    if (!(TalkDemo_GetFlags(0) & 2))
        TalkDemoDim_Create();
    TalkDemo_LoadSound(w);
    TalkDemo_PlayBGM(w);
    w->fn = TalkDemo_State_WaitWindowOpen;
}
#pragma thumb reset
