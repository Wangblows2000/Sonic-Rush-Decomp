#include <ov000_b/stageobj.h>

extern void *gZoneActArchive;
extern u8 data_ov000_02367e90[], data_ov000_0236aa28[];

void func_ov000_0232e90c(UnkStruct_EffObj *w);

void func_ov000_02328d3c(UnkStruct_StageObj *arg, s16 x, s16 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02367e90, 8, data_ov000_0236aa28, gZoneActArchive);
    StageObj_AllocSpritePalette(w, 0, 0x25);
    StageObj_SetOAMOrder(w, 4);
    StageObj_SetOAMPriority(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10 + (x << 8);
    w->unk14 = arg->unk14 + (y << 8);
    w->unk88 = arg;
    w->unk2E = x;
    w->unk30 = y;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    func_ov000_0232e90c(w);
}
