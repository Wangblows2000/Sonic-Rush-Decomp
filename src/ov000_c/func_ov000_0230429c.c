#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230429c;

extern void *data_ov000_02369ba0;

void func_ov000_02304270();
void func_ov000_02304280();
void func_ov000_02342780();

UnkStruct_0230429c *func_ov000_0230429c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230429c *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_02304280, 0, 0x500, 2, sizeof(UnkStruct_0230429c));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230429c));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk8 = 0;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    func_ov000_02304270(w);
    func_ov000_02342780();
    return w;
}
