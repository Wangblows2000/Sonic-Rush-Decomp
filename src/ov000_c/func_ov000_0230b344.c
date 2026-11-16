#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230b344;

extern void *data_ov000_02369ba0;
typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_02366968[];
extern u8 data_ov000_02369f54[];

void func_ov000_022fb9c8();
void func_ov000_0230ac58();
void func_ov000_0230b320();

UnkStruct_0230b344 *func_ov000_0230b344(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230b344 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0230b344));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230b344));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366968, 0x20, data_ov000_02369f54, data_ov000_02369ba0);
    func_0206a054(w, 0, 4);
    func_02069918(w, 0x14);
    if (data_022c4560.unk1C == 2 && data_022c4560.unk1D != 0)
        func_020698f0(w, 1);
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = func_ov000_0230ac58;
    w->base.unk58 |= 0x2100;
    w->base.unk1DC = func_0203663c();
    func_ov000_0230b320(w);
    return w;
}
