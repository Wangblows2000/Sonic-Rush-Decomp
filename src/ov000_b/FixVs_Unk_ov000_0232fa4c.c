#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_0236822c[], data_ov000_0236a9a8[];

void FixVs_State_Unk_ov000_0232fa00(void);

void FixVs_Unk_ov000_0232fa4c(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xf6, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_0236822c, 0xffff, data_ov000_0236a9a8, gActComArchive);
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    StageObj_SetAnimation(w, 4);
    w->unk5C |= 4;
    w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14 - 0x2000;
    w->unk88 = arg;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = FixVs_State_Unk_ov000_0232fa00;
}
