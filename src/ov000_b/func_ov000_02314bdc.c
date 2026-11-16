#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366e90[], data_ov000_0236a224[];

void func_ov000_022fb9c8(void);
void func_ov000_02314bb8(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02314bdc(s32 a, s32 b, s32 c) {
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
    func_02069f1c(w, data_ov000_02366e90, 8, data_ov000_0236a224, data_ov000_02369ba0);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    func_0206a054(w, 0, 2);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->unk54 |= 2;
    w->unk54 |= 0x10;
    w->unk58 |= 0x2100;
    func_ov000_02314bb8(w);
    return w;
}
