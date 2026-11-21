#include <nitro/types.h>

typedef struct { u8 pad[0x10]; u8 *work; } UnkStruct_022e05f0;
extern UnkStruct_022e05f0 *data_022c4db0;
extern s16 gTransformDemoId;
extern u8 data_ov001_023161ec[];
extern u8 data_ov001_023161ed[];
UnkStruct_022e05f0 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
BOOL StoryProgress_IsStageNew(void);
void TransformDemo_Main(void);
void TransformDemo_Destructor(void);
void TransformDemo_State_Init(void);
void TransformDemo_FadeState_Init(void *p, void *q);
void TransformDemo_LoadSound(void *p, void *q);

void TransformDemo_Create(void) {
    UnkStruct_022e05f0 *task = Task_Create(TransformDemo_Main, TransformDemo_Destructor, 0, 0x4000, 3, 0xeb4);
    u8 *w;
    vu16 zero;
    data_022c4db0 = task;
    if (task == (UnkStruct_022e05f0 *)OS_GetArenaLo(0)) {
        return;
    }
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, 0xeb4);
    *(s32 *)(w + 0xbd4) = data_ov001_023161ec[gTransformDemoId * 2];
    *(s32 *)(w + 0x10) = data_ov001_023161ed[gTransformDemoId * 2];
    *(void (**)(void))w = TransformDemo_State_Init;
    if (!StoryProgress_IsStageNew()) {
        *(u32 *)(w + 4) |= 0x100;
    }
    TransformDemo_FadeState_Init(w, w + 0xea0);
    TransformDemo_LoadSound(w, w + 0xea8);
}
