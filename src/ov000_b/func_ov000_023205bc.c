#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023672f4[], data_ov000_0236a720[], data_ov000_0236a8a0[];

void func_ov000_0232053c(void);

UnkStruct_EffObj *func_ov000_023205bc(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0x4ff, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023672f4, 0, data_ov000_0236a720, data_ov000_02369ba0);
    func_02069eb0(w, 0x10, data_ov000_0236a8a0);
    func_02069918(w, 0x15);
    func_0206a054(w, 0, 0x5c);
    func_02069818(w, 1);
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk5C |= 1;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_0232053c;
    return w;
}
