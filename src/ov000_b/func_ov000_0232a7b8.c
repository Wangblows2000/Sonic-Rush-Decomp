#include <ov000_b/stageobj.h>

extern void *gZoneActArchive;
extern u8 data_ov000_02367f78[], data_ov000_0236aa20[];

void func_ov000_0232a5f8(void);

void func_ov000_0232a7b8(UnkStruct_StageObj *arg, s32 x, s32 y, s32 idx) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02367f78, 2, data_ov000_0236aa20, gZoneActArchive);
    StageObj_SetAnimation(w, (u16)(idx + 4));
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    w->spr0.unk2C |= 0x10;
    w->spr1.unk2C |= 0x10;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    w->unk54 |= 0x10;
    w->unk5C |= 1;
    w->unk2E = x << 8;
    w->unk30 = y << 8;
    w->unk10 = arg->unk10 + w->unk2E;
    w->unk14 = arg->unk14 + w->unk30;
    w->unk88 = arg;
    w->unk68 = func_ov000_0232a5f8;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
}
