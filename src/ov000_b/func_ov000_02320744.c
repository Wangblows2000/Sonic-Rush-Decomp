#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_0236730c[], data_ov000_0236a720[], data_ov000_0236a890[];

void func_ov000_0232053c(void);
void func_ov000_023206f0(void);

UnkStruct_EffObj *func_ov000_02320744(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0x4ff, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_0236730c, 0, data_ov000_0236a720, data_ov000_02369ba0);
    func_02069eb0(w, 0xffff, data_ov000_0236a890);
    func_020698f0(w, 1);
    func_02069918(w, 5);
    func_0206a054(w, 0, 0x5c);
    func_02069818(w, 2);
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14;
    w->unk88 = arg;
    w->unk5C |= 1;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_0232053c;
    w->unk70 = func_ov000_023206f0;
    return w;
}
