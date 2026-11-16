#include <ov000_b/stageobj.h>

extern void *gZoneActArchive;
extern u8 data_ov000_02367efc[], data_ov000_0236aa20[];

void func_ov000_02334264(UnkStruct_EffObj *w);

void func_ov000_0232929c(UnkStruct_EffObj *arg, s32 x, s32 y, s32 base) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02367efc, 0x18, data_ov000_0236aa20, gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 7);
    StageObj_SetAnimation(w, 2);
    StageObj_SetOAMOrder(w, 9);
    StageObj_SetOAMPriority(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10 + (x << 8);
    w->unk14 = (base << 8) + y * 256;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
}
