#include <nitro/types.h>

typedef struct UnkWork_02062c9c {
    void (*fn)(struct UnkWork_02062c9c *w);
    u8 pad4[0x64];
    void *unk68;
    u8 pad6C[4];
    s32 unk70;
    u8 pad74[4];
    s32 unk78;
    u8 pad7C[0x64];
} UnkWork_02062c9c;
typedef struct { u8 pad0[0x10]; UnkWork_02062c9c *work; } UnkTask_02062c9c;
typedef struct { u8 pad0[8]; s32 unk8; u8 padC[4]; s32 unk10; } UnkInfo_02062c9c;
extern UnkTask_02062c9c *data_022c4da8;
extern UnkTask_02062c9c *gTaskCurrent;

UnkTask_02062c9c *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void TalkDemoScene_Main(void);
void TalkDemoScene_Destructor(void);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void *TalkDemo_GetScript(void);
void Task_Destroy(UnkTask_02062c9c *t);
void TalkDemoScene_CreateBg2WindowTask(void);
UnkInfo_02062c9c *TalkDemo_GetScriptHeader(void *p);
u32 TalkDemo_GetFlags(s32 a);
void TalkDemoScene_State_FullImage(UnkWork_02062c9c *w);
void TalkDemoScene_State_Init(UnkWork_02062c9c *w);

#pragma thumb on
void TalkDemoScene_Create(void) {
    UnkTask_02062c9c *t;
    UnkWork_02062c9c *w;
    UnkInfo_02062c9c *info;
    vu16 zero;

    t = Task_Create(TalkDemoScene_Main, TalkDemoScene_Destructor, 7, 0xeffe, 3, sizeof(UnkWork_02062c9c));
    data_022c4da8 = t;
    if (t != (UnkTask_02062c9c *)OS_GetArenaLo(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02062c9c));
        w->unk68 = TalkDemo_GetScript();
        if (w->unk68 == NULL) {
            Task_Destroy(gTaskCurrent);
            return;
        }
        info = TalkDemo_GetScriptHeader(w->unk68);
        w->unk70 = info->unk8;
        w->unk78 = info->unk10;
        TalkDemoScene_CreateBg2WindowTask();
        if (TalkDemo_GetFlags(0) & 2)
            w->fn = TalkDemoScene_State_FullImage;
        else
            w->fn = TalkDemoScene_State_Init;
    }
}
#pragma thumb reset
