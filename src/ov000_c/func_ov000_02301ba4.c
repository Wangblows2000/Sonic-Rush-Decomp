#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_02301ba4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023666d0[], data_ov000_02369e44[];

void func_ov000_022fb9c8(void);
void func_ov000_02301ac4(void);
BOOL func_ov000_02345758(s32 x, s32 y);

UnkStruct_02301ba4 *func_ov000_02301ba4(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02301ba4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_02301ba4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02301ba4));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_023666d0, 0xffff, data_ov000_02369e44, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x2a);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e3c0(&w->base.coll0, p->unk6, p->unk7 - 0x60, p->unk8 + p->unk6, p->unk9 + p->unk7);
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unkA |= 0xc0;
    w->base.coll0.unk1C = func_ov000_02301ac4;
    w->base.coll0.unk18 = w;
    w->base.unk58 |= 0x2100;
    if (func_ov000_02345758(w->base.unk10, w->base.unk14))
        w->base.unk5C |= 2;
    return w;
}
