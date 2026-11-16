#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2CC - 0x20C];
} UnkStruct_0231b5c8;

typedef struct {
    u8 pad0[0x1C];
    u8 unk1C;
} UnkStruct_022c4560;

extern void *data_ov000_02369ba0;
extern UnkStruct_022c4560 data_022c4560;
extern u8 data_ov000_023671e8[], data_ov000_0236a348[], data_ov000_0236a360[], data_ov000_0236a350[], data_ov000_0236a370[];
extern u8 data_ov000_023644c4[], data_ov000_023661c0[];

void func_ov000_022fb9c8(void);
void func_ov000_0231b1e8(void);
void func_0204423c(s32 a, void *b, u32 c, s32 d);
BOOL func_ov000_02345758(s32 x, s32 y);
void func_ov000_0231b568(UnkStruct_0231b5c8 *w);

UnkStruct_0231b5c8 *func_ov000_0231b5c8(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231b5c8 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0xf6, 2, sizeof(UnkStruct_0231b5c8));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231b5c8));
    func_ov000_022fba50(w, a, b, c);
    func_02069f1c(w, data_ov000_023671e8, 0, data_ov000_0236a348, data_ov000_02369ba0);
    func_0206a054(w, 0, 6);
    switch (w->base.unk1E0->unk4 & 3) {
    case 0:
        func_02069eb0(w, 0x10, data_ov000_0236a360);
        break;
    case 1:
        func_02069eb0(w, 0x10, data_ov000_0236a350);
        break;
    case 2:
    default:
        func_02069eb0(w, 0x10, data_ov000_0236a370);
        break;
    }
    if (data_022c4560.unk1C == 3) {
        func_0204423c(w->base.spr0.unk44, data_ov000_023644c4, *(u32 *)data_ov000_0236a348, 0);
        w->base.unk54 |= 0x800;
    }
    w->base.spr0.unk46 = 0x14;
    w->base.spr1.unk46 = 0x14;
    func_0206e2b4(&w->base.coll0, 0, -0x10);
    w->base.coll0.unk1C = func_ov000_0231b1e8;
    w->base.coll0.unkA |= 0x800;
    w->base.unk1B8 = w;
    w->base.unk1C8 = data_ov000_023661c0;
    w->base.unk1DA = 6;
    w->base.unk1D6 = -0x1c;
    switch (w->base.unk1E0->unk4 & 3) {
    case 0:
        w->base.unk1D8 = 1;
        w->base.unk1D4 = -3;
        w->base.unk1B8 = NULL;
        break;
    case 1:
        w->base.unk1D8 = 4;
        w->base.unk1D4 = -0x10;
        break;
    case 2:
    default:
        w->base.unk1D8 = 4;
        w->base.unk1D4 = -0x10;
        break;
    }
    if (func_ov000_02345758(w->base.unk10, w->base.unk14))
        w->base.unk5 = 0x80;
    w->base.unk58 |= 0x2100;
    func_ov000_0231b568(w);
    return w;
}
