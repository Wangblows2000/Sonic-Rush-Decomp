#include <ov000_c/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366b60[], data_ov000_0236a01c[];

void func_ov000_0230d804(void);

void func_ov000_0230d930(UnkStruct_StageObj *par, s32 vx, s32 vy, u32 k) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_0206bef4, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    func_02069f1c(w, data_ov000_02366b60, 2, data_ov000_0236a01c, data_ov000_02369ba0);
    if (k >= 6)
        func_0206a054(w, 8, 0x17);
    else
        func_0206a054(w, 0, 2);
    func_02069818(w, (u16)(k + 2));
    func_02069918(w, 0x15);
    w->unk5C |= 1;
    w->unk5C |= par->unk5C & 2;
    w->unk88 = par;
    w->unk10 = par->unk10;
    w->unk14 = par->unk14;
    w->unk2E = vx;
    w->unk30 = vy;
    w->unk58 |= 0x2100;
    w->unk54 |= 0x12;
    w->unk68 = func_ov000_0230d804;
}
