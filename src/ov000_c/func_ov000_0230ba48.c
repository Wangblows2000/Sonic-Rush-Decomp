#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[9];
} UnkStruct_0230ba48;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366984[], data_ov000_02369f64[], data_ov000_02369f5c[], data_ov000_0236699c[];
extern u8 data_ov000_023661c0[], data_ov000_023661a4[];

void func_ov000_0230ba28(void);
void func_ov000_0230b9f4(UnkStruct_0230ba48 *w);

UnkStruct_0230ba48 *func_ov000_0230ba48(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230ba48 *w;
    vu16 zero;
    u16 i;

    task = func_020372d0(func_0206c3d0, func_ov000_0230ba28, 0, 0x500, 2, sizeof(UnkStruct_0230ba48));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230ba48));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366984, 0xffff, data_ov000_02369f64, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x4e);
    func_02069918(w, 0x14);
    for (i = 0; i < 9; i++)
        func_02069a34(w, &w->blk[i], data_ov000_0236699c, 0, i, i, data_ov000_02369f5c, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    func_0206e3c0(&w->base.coll0, -0xa4, -0x50, -0x80, 0x10);
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D0 = data_ov000_023661a4;
    w->base.unk1D8 = 6;
    w->base.unk1DA = 3;
    w->base.unk1D4 = -0x18;
    w->base.unk1D6 = 2;
    w->base.unk58 |= 0x2100;
    w->base.unk204 = 0;
    func_ov000_0230b9f4(w);
    return w;
}
