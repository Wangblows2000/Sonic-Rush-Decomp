#include <game/Stage/stageobj_alt.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366dc4[], data_ov000_0236a1b4[];

void MapObject_Destructor(void);
void Duct_Action_Init(UnkStruct_StageObj *w);

UnkStruct_StageObj *Duct_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366dc4, 0xffff, data_ov000_0236a1b4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x2d);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk58 |= 0x2100;
    w->unk1DC = Sound_AllocHandle();
    Duct_Action_Init(w);
    return w;
}
