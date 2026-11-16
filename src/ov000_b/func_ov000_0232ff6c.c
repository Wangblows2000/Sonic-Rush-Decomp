#include <ov000_b/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_02368270[], data_ov000_0236a9b8[];

void func_ov000_02334264(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_0232ff6c(s32 x, s32 y) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02368270, 0xffff, data_ov000_0236a9b8, data_022c448c);
    func_0206a054(w, 4, 0x17);
    func_02069818(w, 4);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->unk5C |= 1;
    w->unk10 = x;
    w->unk14 = y;
    w->unk58 |= 0x100;
    w->unk58 |= 0x2000;
    w->unk54 |= 2;
    func_ov000_02334264(w);
    return w;
}
