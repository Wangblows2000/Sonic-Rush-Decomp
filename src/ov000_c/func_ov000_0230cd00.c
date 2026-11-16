#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366b04[], data_ov000_0236a010[];

void func_ov000_0230ccd4(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_0230cd00(UnkStruct_StageObj *par) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0x500, 2, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02366b04, 0x20, data_ov000_0236a010, data_ov000_02369ba0);
    func_0206a054(w, 1, 0x66);
    w->spr0.unk46 = 0x13;
    w->spr1.unk46 = 0x13;
    w->unk88 = par;
    w->unk10 = ((UnkStruct_StageObj *)w->unk88)->unk10;
    w->unk14 = ((UnkStruct_StageObj *)w->unk88)->unk14;
    w->unk58 |= 0x2100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    func_ov000_0230ccd4(w);
    return w;
}
