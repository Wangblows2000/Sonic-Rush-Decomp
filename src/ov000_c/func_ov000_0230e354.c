#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366b9c[], data_ov000_0236a02c[];

void func_ov000_0230e238(void);

void func_ov000_0230e354(void *par, s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02366b9c, 0x20, data_ov000_0236a02c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x32);
    func_02069818(w, 1);
    func_02069918(w, 0x16);
    w->unk5C |= 1;
    w->unk88 = par;
    w->unk10 = x;
    w->unk14 = y;
    w->unkC = 0xb6;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_0230e238;
}
