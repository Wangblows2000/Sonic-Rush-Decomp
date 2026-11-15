#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023670ac[], data_ov000_0236a2ac[];

UnkStruct_EffObj *func_ov000_02318cc4(UnkStruct_StageObj *arg, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023670ac, 0xffff, data_ov000_0236a2ac, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x48);
    func_02069918(w, 0x13);
    func_020698f0(w, 1);
    w->unk5C |= arg->unk5C & 1;
    w->unk5C |= arg->unk5C & 2;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    if (n) {
        func_02069818(w, 2);
        if (w->unk5C & 1)
            w->unk10 -= 0x400;
        else
            w->unk10 += 0x400;
    } else {
        func_02069818(w, 0);
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
