#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_023680b8[], data_ov000_0236a9c0[];

void func_ov000_0232dbfc(void);
void func_ov000_0232d95c(void);

void func_ov000_0232d9a4(void) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_0232dbfc, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023680b8, 0xffff, data_ov000_0236a9c0, data_022c448c);
    w->spr0.unk44 = 0;
    w->spr1.unk44 = 0;
    func_02069918(w, 0);
    func_020698f0(w, 0);
    w->spr0.unk2C |= 0x80000;
    w->spr1.unk2C &= ~0x10;
    w->unk10 = 0x400;
    w->unk14 = 0x400;
    w->unk5C |= 1;
    w->unk5C |= 0x80;
    w->unk58 |= 0x8000100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_0232d95c;
}
