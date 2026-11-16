#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_0236694c[], data_ov000_02369f54[];

void GoalRingPart_Action_FollowRing(UnkStruct_EffObj *w);

void GoalRing_CreatePart_ov000_0230ab48(UnkStruct_StageObj *par) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_0236694c, 0xa, data_ov000_02369f54, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 4);
    w->spr0.unk46 = 9;
    w->spr1.unk46 = 9;
    if (par->unk5C & 1)
        w->unk5C |= 1;
    w->unk88 = par;
    w->unk10 = par->unk10;
    w->unk14 = par->unk14;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    GoalRingPart_Action_FollowRing(w);
}
