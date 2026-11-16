#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366b04[], data_ov000_0236a010[];

void IronBallPart_Action_Unk_ov000_0230ccd4(UnkStruct_EffObj *w);

UnkStruct_EffObj *IronBall_CreatePart_ov000_0230cd00(UnkStruct_StageObj *par) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0x500, 2, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366b04, 0x20, data_ov000_0236a010, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 1, 0x66);
    w->spr0.unk46 = 0x13;
    w->spr1.unk46 = 0x13;
    w->unk88 = par;
    w->unk10 = ((UnkStruct_StageObj *)w->unk88)->unk10;
    w->unk14 = ((UnkStruct_StageObj *)w->unk88)->unk14;
    w->unk58 |= 0x2100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    IronBallPart_Action_Unk_ov000_0230ccd4(w);
    return w;
}
