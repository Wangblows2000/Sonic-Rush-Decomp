#include <ov000_b/stageobj.h>

extern void *data_022c4488;
extern u8 data_ov000_023681f4[], data_ov000_0236a9c8[];

void func_ov000_0232f53c(void);

void func_ov000_0232f5a8(UnkStruct_StageObj *arg) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xf6, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023681f4, 0xffff, data_ov000_0236a9c8, data_022c4488);
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    func_02069918(w, 3);
    func_020698f0(w, 0);
    func_02069818(w, 1);
    w->unk5C |= 4;
    w->unk5C |= 1;
    w->unk10 = arg->unk10;
    w->unk14 = arg->unk14 - 0x2000;
    w->unk88 = arg;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_0232f53c;
}
