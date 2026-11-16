#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x104];
} UnkStruct_0230be44_Sub;

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_0230be44_Sub sub;
} UnkStruct_0230be44;

extern void *data_ov000_02369ba0;
extern UnkStruct_Task *data_ov000_02369f70;
extern u8 data_ov000_023669d0[], data_ov000_02369fa4[], data_ov000_02369fdc[], data_ov000_02369f8c[];
extern u8 data_ov000_023669e8[], data_ov000_02369fec[];

void MapObj13Part_Destructor_ov000_0230bdf4(void);
void MapObj13Part_State_Unk_ov000_0230be10(void);
void StageObj_Load3dBAC(void *w, UnkStruct_0230be44_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_02068358(UnkStruct_0230be44_Sub *s, void *a, s32 b, s32 c);

UnkStruct_0230be44 *MapObj13_CreatePart_ov000_0230be44(void) {
    UnkStruct_Task *task;
    UnkStruct_0230be44 *w;
    vu16 zero;

    if (data_ov000_02369f70 != NULL)
        return data_ov000_02369f70->work;
    task = Task_Create(StageObj_Main, MapObj13Part_Destructor_ov000_0230bdf4, 0, 0x3000, 4, sizeof(UnkStruct_0230be44));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    data_ov000_02369f70 = task;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230be44));
    StageObj_Load2dBAC(w, data_ov000_023669d0, 0, data_ov000_02369fa4, data_ov000_02369ba0);
    StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_02369fdc);
    w->base.spr0.unk2C |= 0x80000;
    StageObj_Load3dBAC(w, &w->sub, data_ov000_023669e8, 0, 0, data_ov000_02369f8c, data_ov000_02369ba0);
    func_02068358(&w->sub, data_ov000_02369fec, 0x800, 0x10);
    StageObj_AllocSpritePalette(w, 0, 0xe);
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->base.unk5C |= 4;
    w->base.unk54 |= 0x10;
    w->base.unkC = 1;
    w->base.unk68 = MapObj13Part_State_Unk_ov000_0230be10;
    return w;
}
