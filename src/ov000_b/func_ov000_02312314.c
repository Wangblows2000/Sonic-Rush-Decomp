#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366d4c[], data_ov000_0236a164[], data_ov000_023661c0[];

void func_ov000_022fb9c8(void);
void func_ov000_02312214(void);
void func_ov000_023121e4(void);
void func_ov000_02312304(UnkStruct_StageObj *w);

UnkStruct_StageObj *func_ov000_02312314(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_StageObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    func_ov000_022fba50(w, (s32)p, b, c);
    func_02069f1c(w, data_ov000_02366d4c, 0xffff, data_ov000_0236a164, data_ov000_02369ba0);
    func_0206a054(w, 0, 0x31);
    w->spr0.unk46 = 6;
    w->spr1.unk46 = 6;
    w->spr0.unk2C &= ~0x60000;
    w->spr1.unk2C &= ~0x60000;
    if (w->unk1E0->unk4 & 8)
        w->unk1E0->unk4 &= ~4;
    switch (p->unk2) {
    case 0xab:
        w->unk1E0->unk4 &= ~1;
        w->unk1E0->unk4 |= 2;
        break;
    case 0xac:
        w->unk1E0->unk4 |= 1;
        w->unk1E0->unk4 &= ~2;
        break;
    case 0xad:
        w->unk1E0->unk4 |= 1;
        w->unk1E0->unk4 |= 2;
        break;
    }
    if (w->unk1E0->unk4 & 1) {
        if (w->unk1E0->unk4 & 4)
            func_02069818(w, 3);
        else
            func_02069818(w, 2);
    } else if (w->unk1E0->unk4 & 4) {
        func_02069818(w, 1);
    }
    if (w->unk1E0->unk4 & 2) {
        if (w->unk1E0->unk4 & 1)
            w->unk5C ^= 1;
        else
            w->unk5C ^= 2;
    }
    func_0206e2b4(&w->coll0, 0, -0x10);
    w->coll0.unk1C = func_ov000_02312214;
    w->coll0.unkA |= 0x800;
    w->coll1.unk18 = NULL;
    func_0206e3c0(&w->coll1, -2, -2, 2, 2);
    w->coll1.unkA |= 0x800;
    func_0206e2b4(&w->coll1, 0, -0x10);
    w->coll1.unk28 = 0x12;
    w->coll1.unk1C = func_ov000_023121e4;
    w->coll1.unk18 = w;
    w->unk1B8 = w;
    w->unk1C8 = data_ov000_023661c0;
    if (w->unk1E0->unk4 & 1) {
        w->unk1D8 = 3;
        w->unk1DA = 5;
        w->unk1D4 = 0xd;
        w->unk1D6 = -0x12;
    } else {
        w->unk1D8 = 5;
        w->unk1DA = 4;
        w->unk1D4 = -0x14;
        w->unk1D6 = -8;
    }
    w->unk58 |= 0x2100;
    func_ov000_02312304(w);
    return w;
}
