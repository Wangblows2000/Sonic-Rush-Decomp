#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x104];
} UnkStruct_0230c02c_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_0230c02c_Sub sub;
} UnkStruct_0230c02c;

extern void *data_ov000_02369ba0;
extern void *gActComArchive;
extern UnkStruct_Task *data_ov000_02369f6c;
extern u8 data_ov000_02366a04[], data_ov000_02369f84[], data_ov000_02369fcc[], data_ov000_02369f7c[];
extern u8 data_ov000_02366a1c[], data_ov000_02369ffc[];

void MapObj13Part_Destructor_ov000_0230bfdc(void);
void MapObj13Part_State_DestroyWhenIdle2(void);
void StageObj_Load3dBAC(void *w, UnkStruct_0230c02c_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void StageObj_AcquireSharedTexture(UnkStruct_0230c02c_Sub *s, void *a, s32 b, s32 c);

UnkStruct_0230c02c *MapObj13_CreatePart_ov000_0230c02c(void) {
    UnkStruct_Task *task;
    UnkStruct_0230c02c *w;
    vu16 zero;

    if (data_ov000_02369f6c != NULL)
        return data_ov000_02369f6c->work;
    task = Task_Create(StageObj_Main, MapObj13Part_Destructor_ov000_0230bfdc, 0, 0x3000, 4, sizeof(UnkStruct_0230c02c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    data_ov000_02369f6c = task;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230c02c));
    StageObj_Load2dBAC(w, data_ov000_02366a04, 0xffff, data_ov000_02369f84, gActComArchive);
    StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_02369fcc);
    w->base.spr0.unk2C |= 0x80000;
    StageObj_Load3dBAC(w, &w->sub, data_ov000_02366a1c, 0, 0, data_ov000_02369f7c, data_ov000_02369ba0);
    StageObj_AcquireSharedTexture(&w->sub, data_ov000_02369ffc, 0x80, 0x10);
    w->base.spr0.unk44 = 2;
    w->base.spr1.unk44 = 2;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->base.unk5C |= 4;
    w->base.unk54 |= 0x10;
    w->base.unkC = 1;
    w->base.unk68 = MapObj13Part_State_DestroyWhenIdle2;
    return w;
}
