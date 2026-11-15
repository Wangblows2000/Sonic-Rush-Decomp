#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
} UnkStruct_0232dc20;

extern void *data_022c448c;
extern u8 data_ov000_023680d8[], data_ov000_0236a9a8[], data_ov000_023680ec[];

void func_ov000_0232dbfc(void);
void func_ov000_0232dad8(void);

void func_ov000_0232dc20(void) {
    UnkStruct_Task *task;
    UnkStruct_0232dc20 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_0232dbfc, 0, 0xa00, 4, sizeof(UnkStruct_0232dc20));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232dc20));
    func_02069f1c(w, data_ov000_023680d8, 0xffff, data_ov000_0236a9a8, data_022c448c);
    w->base.spr0.unk44 = 1;
    w->base.spr1.unk44 = 1;
    func_02069918(w, 3);
    func_020698f0(w, 0);
    func_02069d38(&w->s0, &w->s1, data_ov000_023680ec, 0xffff, data_ov000_0236a9a8, data_022c448c);
    func_0206990c(&w->s0, &w->s1, 3);
    func_020698bc(&w->s0, &w->s1, 0);
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    w->s0.unk2C |= 0x200;
    w->s1.unk2C |= 0x200;
    func_02033550(&w->s0, 2);
    func_02033550(&w->s1, 2);
    w->base.unk70 = func_ov000_0232dad8;
    w->base.unk10 = 0xc800;
    w->base.unk14 = 0x400;
    w->base.unk5C |= 1;
    w->base.unk5C |= 0x80;
    w->base.unk58 |= 0x8000100;
    w->base.unk54 |= 0x12;
}
