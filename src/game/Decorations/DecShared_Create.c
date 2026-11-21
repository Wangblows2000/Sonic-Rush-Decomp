#include <game/Stage/stageobj_alt.h>

typedef struct {
    u8 pad0[8];
} UnkStruct_0236a6f0;

typedef struct {
    u8 pad0[0x10];
} UnkStruct_0236a790;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_Task *gDecSharedObjects[];
extern UnkStruct_0236a6f0 data_ov000_0236a6f0[];
extern UnkStruct_0236a790 data_ov000_0236a790[];

void DecShared_Destructor(void);
void DecShared_State_Main(void);
void AnimatorSprite_AnimateManual(UnkStruct_Spr *s, s16 a, s32 b, s32 c);

void *DecShared_Create(void *a, s32 b, s32 c, s32 d, u16 e) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;
    s32 i = e;

    if (gDecSharedObjects[i] != NULL)
        return gDecSharedObjects[i]->work;
    task = Task_Create(StageObj_Main, DecShared_Destructor, 0, 0x3000, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    gDecSharedObjects[i] = task;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, a, 0, &data_ov000_0236a6f0[d], data_ov000_02369ba0);
    StageObj_SetSharedSpriteVram(w, b, &data_ov000_0236a790[e]);
    StageObj_SetAnimation(w, c);
    w->unk58 |= 0x2100;
    w->unk5C |= 0x20;
    w->unk5C |= 4;
    w->unk54 |= 0x10;
    w->unkC = 1;
    w->unk8 = (u16)e;
    w->unk68 = DecShared_State_Main;
    w->spr0.unk2C |= 0x10;
    w->spr1.unk2C |= 0x10;
    AnimatorSprite_AnimateManual(&w->spr0, (gGameState.unk20 & 0xf) << 4, 0, 0);
    AnimatorSprite_AnimateManual(&w->spr1, (gGameState.unk20 & 0xf) << 4, 0, 0);
    return w;
}
