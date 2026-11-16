#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Blk blk[5];
} UnkStruct_0230ca50;

typedef struct {
    u8 b[5];
} UnkStruct_0230ca50_Tbl;

extern void *data_ov000_02369ba0;
extern UnkStruct_0230ca50_Tbl data_ov000_023669b0;
extern u8 data_ov000_02366ad4[], data_ov000_02369f94[], data_ov000_02369f9c[], data_ov000_02366aec[];

void func_ov000_022fb9c8(void);
void func_ov000_0230c814(void);
void func_ov000_0230ca40(UnkStruct_0230ca50 *w);

UnkStruct_0230ca50 *func_ov000_0230ca50(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_0230ca50_Tbl tbl = data_ov000_023669b0;
    UnkStruct_Task *task;
    UnkStruct_0230ca50 *w;
    vu16 zero;
    u16 i;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x800, 2, sizeof(UnkStruct_0230ca50));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230ca50));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366ad4, 0xffff, data_ov000_02369f94, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x51);
    func_02069918(w, 0x14);
    w->base.spr0.unk2C |= 0x80000;
    for (i = 0; i < 5; i++)
        func_02069a34(w, &w->blk[i], data_ov000_02366aec, 0, i, tbl.b[i], data_ov000_02369f9c, data_ov000_02369ba0);
    w->base.unk5C |= 0x200;
    Obj_SetVec(&w->base.unk128->unk18, 0x34cc, 0x34cc, 0x34cc);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e3c0(&w->base.coll0, -16, -36, 16, 32);
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0x800;
    w->base.coll0.unk1C = func_ov000_0230c814;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2100;
    w->base.unk204 = 0;
    func_ov000_0230ca40(w);
    return w;
}
