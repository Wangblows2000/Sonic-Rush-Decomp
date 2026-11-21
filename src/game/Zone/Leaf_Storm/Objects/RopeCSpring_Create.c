#include <game/Stage/stageobj_alt.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366e90[], data_ov000_0236a224[];

void MapObject_Destructor(void);
void RopeCSpring_Action_Idle(UnkStruct_StageObj *w);

UnkStruct_StageObj *RopeCSpring_Create(s32 a, s32 b, s32 c) {
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
    StageObj_Load2dBAC(w, data_ov000_02366e90, 8, data_ov000_0236a224, data_ov000_02369ba0);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    StageObj_AllocSpritePalette(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk58 |= 0x2100;
    RopeCSpring_Action_Idle(w);
    return w;
}
