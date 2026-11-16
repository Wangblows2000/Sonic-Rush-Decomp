#include <ov000_c/stageobj.h>

extern void *data_022c448c;
extern u8 data_ov000_023669b8[], data_ov000_02369f84[];

void func_ov000_0230bc38(void);

UnkStruct_EffObj *func_ov000_0230bc50(UnkStruct_StageObj *par) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_023669b8, 0xffff, data_ov000_02369f84, data_022c448c);
    func_0206a054(w, 0, 0x67);
    func_02069818(w, 1);
    func_02069918(w, 9);
    func_020698f0(w, 1);
    w->unk5C |= 1;
    w->unk10 = par->unk10;
    w->unk14 = par->unk14;
    w->unk58 |= 0x100;
    w->unk54 |= 2;
    w->unk68 = func_ov000_0230bc38;
    return w;
}
