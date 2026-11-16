#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
} UnkStruct_0230e8e4;

extern void *data_ov000_02369ba0;
extern void *data_ov000_0236a024;
extern u8 data_ov000_02366bbc[], data_ov000_0236a02c[], data_ov000_02366bdc[];

void func_ov000_0230e8c0(void);
void func_ov000_0230e47c(void);
void func_020687b8(void *a, void *b, void *c);
void func_ov000_0230e8b0(UnkStruct_0230e8e4 *w);
void func_ov000_0230e354(UnkStruct_0230e8e4 *w, s32 x, s32 y);

UnkStruct_0230e8e4 *func_ov000_0230e8e4(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230e8e4 *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_0230e8c0, 0, 0xf6, 2, sizeof(UnkStruct_0230e8e4));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230e8e4));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366bbc, 0xffff, data_ov000_0236a02c, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x32);
    func_02069918(w, 0x14);
    w->base.spr0.unk2C |= 0x200;
    w->base.spr1.unk2C |= 0x200;
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = func_ov000_0230e47c;
    w->base.coll0.unkA |= 0x80;
    func_02068e40(w, -20, -5, 0x1c, 9);
    func_020687b8(&data_ov000_0236a024, data_ov000_02366bdc, data_ov000_02369ba0);
    w->base.unk1C8 = data_ov000_0236a024;
    w->base.unk1B8 = w;
    w->base.unk1D8 = 10;
    w->base.unk1DA = 3;
    w->base.unk1D4 = -0x2a;
    w->base.unk1D6 = -0xc;
    w->base.unk1C4 |= 1;
    w->base.unk5C ^= 1;
    w->base.unk58 |= 0x4080;
    func_ov000_0230e8b0(w);
    w->base.unk1DC = func_0203663c();
    func_ov000_0230e354(w, w->base.unk10 + (w->base.unk1E0->unk6 << 8), w->base.unk14 + (w->base.unk1E0->unk7 << 8));
    return w;
}
