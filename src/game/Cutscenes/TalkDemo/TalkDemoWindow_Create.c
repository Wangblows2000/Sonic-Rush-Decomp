#include <nitro/types.h>

typedef struct UnkWork_020602dc {
    void (*fn)(struct UnkWork_020602dc *w);
    u8 pad4[0x18];
    void *unk1C;
    u16 unk20;
    u8 pad22[0xd6];
} UnkWork_020602dc;
typedef struct { u8 pad0[0x10]; UnkWork_020602dc *work; } UnkTask_020602dc;
typedef struct { u8 pad0[0x14]; s32 unk14; } UnkInfo_020602dc;
extern UnkWork_020602dc *data_022c4d9c;
extern UnkTask_020602dc *gTaskCurrent;

UnkTask_020602dc *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void TalkDemoWindow_Main(void);
void TalkDemoWindow_Destructor(void);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void *TalkDemo_GetScript(void);
void Task_Destroy(UnkTask_020602dc *t);
void TalkDemoWindow_InitVAlarms(UnkWork_020602dc *w);
UnkInfo_020602dc *TalkDemo_GetScriptHeader(void *p);
u32 TalkDemo_GetFlags(s32 a);
void TalkDemoWindow_State_InitCenter(UnkWork_020602dc *w);
void TalkDemoWindow_State_InitLow(UnkWork_020602dc *w);

#pragma thumb on
void TalkDemoWindow_Create(void) {
    UnkTask_020602dc *t = Task_Create(TalkDemoWindow_Main, TalkDemoWindow_Destructor, 7, 0xeffe, 3, sizeof(UnkWork_020602dc));
    UnkWork_020602dc *w;
    vu16 zero;

    if (t != (UnkTask_020602dc *)OS_GetArenaLo(0)) {
        w = t->work;
        data_022c4d9c = w;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_020602dc));
        w->unk1C = TalkDemo_GetScript();
        if (w->unk1C == NULL) {
            Task_Destroy(gTaskCurrent);
            return;
        }
        TalkDemoWindow_InitVAlarms(w);
        w->unk20 = TalkDemo_GetScriptHeader(w->unk1C)->unk14;
        if (TalkDemo_GetFlags(0) & 2)
            w->fn = TalkDemoWindow_State_InitCenter;
        else
            w->fn = TalkDemoWindow_State_InitLow;
    }
}
#pragma thumb reset
