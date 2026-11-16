#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x228 - 0x20C];
} UnkStruct_02308904;

extern void *data_ov000_02369ba0;

void func_ov000_02308720();
void func_ov000_023088e8();
void func_ov000_023088f8();

UnkStruct_02308904 *func_ov000_02308904(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02308904 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_023088f8, 0, 0x500, 2, sizeof(UnkStruct_02308904));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02308904));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk4A = 0x300;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    if (w->base.unk1E0->unk4 & 1)
        func_ov000_02308720(w);
    else
        func_ov000_023088e8(w);
    return w;
}
