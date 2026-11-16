#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366e74[], data_ov000_0236a21c[];

void func_ov000_022fb9c8(void);
void func_ov000_0231499c(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_023149c0(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02366e74, 0xc, data_ov000_0236a21c, data_ov000_02369ba0);
    func_0206a054(w, 0, 5);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk54 |= 2;
    w->unk58 |= 0x2100;
    func_ov000_0231499c(w);
    return w;
}
