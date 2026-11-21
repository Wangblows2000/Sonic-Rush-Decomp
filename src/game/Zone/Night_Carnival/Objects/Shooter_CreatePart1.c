#include <game/Stage/stageobj_alt.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366f90[], data_ov000_0236a25c[];

void ShooterPart1_State_Main(void);

UnkStruct_EffObj *Shooter_CreatePart1(UnkStruct_StageObj *arg, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02366f90, 6, data_ov000_0236a25c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x59);
    StageObj_SetOAMOrder(w, 0x12);
    w->unk5C |= arg->unk5C & 1;
    w->unk5C ^= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk8 = 0;
    if (n)
        w->unk54 |= 0x1000;
    if (w->unk54 & 0x1000)
        StageObj_SetAnimation(w, 7);
    else
        StageObj_SetAnimation(w, 2);
    w->unk5C |= 4;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = ShooterPart1_State_Main;
    return w;
}
