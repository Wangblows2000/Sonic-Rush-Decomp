#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x240 - 0x20C];
} UnkStruct_023036d4;

extern void *data_ov000_02369ba0;

void func_ov000_02303548();
void func_ov000_023036b8();
void func_ov000_02342780();

UnkStruct_023036d4 *func_ov000_023036d4(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_023036d4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023036b8, 0, 0x500, 2, sizeof(UnkStruct_023036d4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_023036d4));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    func_ov000_02303548(w);
    func_ov000_02342780();
    return w;
}
