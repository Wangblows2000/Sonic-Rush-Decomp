#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_022ff7dc;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366564[];
extern u8 data_ov000_02369e04[];

void func_ov000_022fb9c8();
void func_ov000_022ff770();

UnkStruct_022ff7dc *func_ov000_022ff7dc(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_022ff7dc *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_022ff7dc));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ff7dc));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366564, 2, data_ov000_02369e04, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x30);
    func_02069818(w, 4);
    w->base.unk5C |= 4;
    func_02069918(w, 0x15);
    if (!(w->base.unk1E0->unk4 & 1))
        w->base.unk5C &= ~1;
    w->base.unk58 |= 0x90;
    w->base.unk5C |= 0x100;
    w->base.unk1DC = func_0203663c();
    w->base.unk68 = func_ov000_022ff770;
    return w;
}
