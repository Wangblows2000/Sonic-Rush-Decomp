#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366b88[], data_ov000_0236a034[];

void func_ov000_0230e030(void);

void func_ov000_0230e0dc(UnkStruct_StageObj *par) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02366b88, 0xffff, data_ov000_0236a034, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x17);
    func_02069918(w, 0x14);
    w->unk88 = par;
    w->unk10 = par->unk10;
    w->unk14 = par->unk14;
    w->unkC = 0;
    if (((UnkStruct_StageObj *)w->unk88)->unk5C & 1)
        w->unk5C |= 1;
    else
        w->unk5C &= ~1;
    w->unk5C |= 4;
    w->unk5C |= 0x100;
    w->unk58 |= 0x40;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk68 = func_ov000_0230e030;
}
