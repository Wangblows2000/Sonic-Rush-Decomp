#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366f90[], data_ov000_0236a25c[];

void func_ov000_02316db0(void);

UnkStruct_EffObj *func_ov000_02316ee8(UnkStruct_StageObj *arg, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02366f90, 6, data_ov000_0236a25c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x59);
    func_02069918(w, 0x12);
    w->unk5C |= arg->unk5C & 1;
    w->unk5C ^= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk8 = 0;
    if (n)
        w->unk54 |= 0x1000;
    if (w->unk54 & 0x1000)
        func_02069818(w, 7);
    else
        func_02069818(w, 2);
    w->unk5C |= 4;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_02316db0;
    return w;
}
