#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; void (*fn)(void); u32 unkC; s32 unk10; } UnkEnt_020735d4;
typedef struct {
    u8 pad0[4];
    void (*fn)(void);   /* 0x4 */
    u8 pad8[4];
    u32 unkC;
    u8 pad10[2];
    u16 unk12;
    s32 unk14;
    u8 pad18;
    u8 unk19;
    u8 pad1A[2];
    s32 unk1C;
} UnkWork_02048734;
typedef struct { u8 pad0[0x10]; UnkWork_02048734 *work; } UnkTask_02048734;
typedef struct { s32 unk0; u8 pad4[0x18]; u8 unk1C; } UnkStruct_022c4560;
typedef struct { u8 pad0[8]; u32 unk8; } UnkStruct_022c4680;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4680 gSaveData;
extern UnkEnt_020735d4 gDemoScriptTable[];
extern UnkTask_02048734 *gTaskCurrent;

UnkTask_02048734 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void DemoScript_Main(void);
void DemoScript_Destructor(void);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
BOOL StoryProgress_IsStageNew(void);
void Task_Destroy(UnkTask_02048734 *t);
void DemoScript_ScheduleNext(UnkWork_02048734 *w);

#pragma thumb on
void DemoScript_Create(s32 mode) {
    UnkTask_02048734 *t = Task_Create(DemoScript_Main, DemoScript_Destructor, 7, 0x100, 3, 0x20);
    UnkWork_02048734 *w;
    u8 stage;
    u8 id;
    UnkEnt_020735d4 *e;
    vu16 zero;

    if (t != (UnkTask_02048734 *)OS_GetArenaLo(0)) {
        w = t->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02048734));
        stage = gGameState.unk1C;
        switch (mode) {
        case 0:
        case 1:
            if (mode == 0)
                id = 0;
            else
                id = 0x20;
            if (gGameState.unk0 == 1)
                id += 0x10;
            if (!StoryProgress_IsStageNew())
                id += 8;
            id += stage;
            break;
        case 3:
            id = 0x40;
            if (gSaveData.unk8 & 0x1000)
                id++;
            break;
        case 4:
            id = 0x42;
            if (gGameState.unk0 == 1)
                id++;
            break;
        case 2:
            id = 0x44;
            break;
        }
        w->unk12 = id;
        e = &gDemoScriptTable[w->unk12];
        w->fn = e->fn;
        w->unk14 = e->unk0;
        w->unk19 = e->unk4;
        w->unkC |= e->unkC;
        w->unk1C = e->unk10;
        if (w->unk19 == 0) {
            void (*fn)(void) = w->fn;
            Task_Destroy(gTaskCurrent);
            fn();
        } else {
            DemoScript_ScheduleNext(w);
        }
    }
}
#pragma thumb reset
