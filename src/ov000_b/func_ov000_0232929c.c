#include <ov000_b/stageobj.h>

extern void *data_022c4488;
extern u8 data_ov000_02367efc[], data_ov000_0236aa20[];

void func_ov000_02334264(UnkStruct_EffObj *w);

void func_ov000_0232929c(UnkStruct_EffObj *arg, s32 x, s32 y, s32 base) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02367efc, 0x18, data_ov000_0236aa20, data_022c4488);
    func_0206a054(w, 0, 7);
    func_02069818(w, 2);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    if (arg->unk5C & 1)
        w->unk5C |= 1;
    w->unk10 = arg->unk10 + (x << 8);
    w->unk14 = (base << 8) + y * 256;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    func_ov000_02334264(w);
}
