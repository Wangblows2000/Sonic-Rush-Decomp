#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023670ac[], data_ov000_0236a2ac[];

UnkStruct_EffObj *Switch_CreatePart(UnkStruct_StageObj *arg, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_023670ac, 0xffff, data_ov000_0236a2ac, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x48);
    StageObj_SetOAMOrder(w, 0x13);
    StageObj_SetOAMPriority(w, 1);
    w->unk5C |= arg->unk5C & 1;
    w->unk5C |= arg->unk5C & 2;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    if (n) {
        StageObj_SetAnimation(w, 2);
        if (w->unk5C & 1)
            w->unk10 -= 0x400;
        else
            w->unk10 += 0x400;
    } else {
        StageObj_SetAnimation(w, 0);
        if (w->unk5C & 2)
            w->unk14 -= 0x400;
        else
            w->unk14 += 0x400;
    }
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    return w;
}
