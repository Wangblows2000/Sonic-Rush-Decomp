#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_02301fe4;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023666e4[], data_ov000_02369e4c[], data_ov000_023661c0[], data_ov000_023661a4[];

void func_ov000_022fb9c8(void);
void func_ov000_02301d2c(void);
void func_ov000_02301fc0(UnkStruct_02301fe4 *w);

UnkStruct_02301fe4 *func_ov000_02301fe4(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02301fe4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_02301fe4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02301fe4));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_023666e4, 0xc, data_ov000_02369e4c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x13);
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = func_ov000_02301d2c;
    w->base.coll0.unkA |= 0x800;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1D0 = data_ov000_023661a4;
    w->base.unk1D8 = 5;
    w->base.unk1DA = 1;
    w->base.unk1D4 = 2;
    w->base.unk1D6 = 0;
    w->base.unk58 |= 0x2100;
    if (w->base.unk1E0->unk2 == 0x57)
        w->base.unk5C &= ~1;
    func_ov000_02301fc0(w);
    return w;
}
