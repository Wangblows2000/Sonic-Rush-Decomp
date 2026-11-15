#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[4];
} UnkStruct_0231bffc;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02367208[], data_ov000_0236a380[];

void func_ov000_022fb9c8(void);
void func_ov000_0231bd44(void);
void func_ov000_0231bfa0(UnkStruct_0231bffc *w);

UnkStruct_0231bffc *func_ov000_0231bffc(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231bffc *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0231bffc));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231bffc));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_02367208, 0x60, data_ov000_0236a380, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x65);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e2b4(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = NULL;
    w->base.coll0.unkA |= 0x800;
    w->base.unk58 |= 0x2100;
    switch (w->base.unk1E0->unk2) {
    case 0x85:
        func_0206e3c0(&w->base.coll1, 0xc, -0x40, 0x24, 0);
        break;
    case 0x86:
        func_0206e3c0(&w->base.coll1, -0x40, -0xc, 0, -0x24);
        w->base.unk5C |= 1;
        w->base.unk5C |= 0x100;
        w->base.unk1 = 0xc0;
        break;
    case 0x87:
        func_0206e3c0(&w->base.coll1, -0x40, -0xc, 0, -0x24);
        w->base.unk5C &= ~1;
        w->base.unk5C |= 0x100;
        w->base.unk1 = 0x40;
        break;
    }
    func_0206e2b4(&w->base.coll1, 0, -0x10);
    w->base.coll1.unkA |= 0xc0;
    w->base.coll1.unkA |= 0x800;
    w->base.coll1.unk1C = func_ov000_0231bd44;
    w->base.coll1.unk18 = w;
    w->base.unk1DC = func_0203663c();
    func_ov000_0231bfa0(w);
    return w;
}
