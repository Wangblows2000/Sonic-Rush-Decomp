#include <ov000_b/stageobj.h>

extern void *gZoneActArchive;
extern u8 data_ov000_02368210[], data_ov000_0236a9c8[];

BOOL Stage_IsGravityFlipped(s32 x, s32 y);
void func_ov000_0232f6f4(void);

void func_ov000_0232f81c(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xf6, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02368210, 0xffff, data_ov000_0236a9c8, gZoneActArchive);
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    if (Stage_IsGravityFlipped(w->unk10, w->unk14))
        w->unk5C |= 2;
    if (w->unk5C & 2) {
        if (arg->unk5C & 1)
            StageObj_SetAnimation(w, 5);
        else
            StageObj_SetAnimation(w, 4);
    } else {
        if (arg->unk5C & 1)
            StageObj_SetAnimation(w, 4);
        else
            StageObj_SetAnimation(w, 5);
    }
    if (w->unk5C & 2)
        w->unk14 = arg->unk14 + 0x2000;
    else
        w->unk14 = arg->unk14 - 0x2000;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_0232f6f4;
}
