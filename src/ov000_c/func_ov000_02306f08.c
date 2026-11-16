#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Spr sprA;
    UnkStruct_Spr sprB;
    u8 *unk29C;
    u32 unk2A0;
    s32 unk2A4;
} UnkStruct_02306f08;

extern void *data_ov000_02369ba0;
extern u8 data_ov000_023668d0[], data_ov000_02369ec4[];

void func_ov000_02306e90(void);
void func_ov000_02306d88(void);
void func_ov000_02306e74(UnkStruct_02306f08 *w);
s32 func_02039c18(s32 a, s32 n);
void func_0203363c(UnkStruct_Spr *s, void *a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, u32 h);

UnkStruct_02306f08 *func_ov000_02306f08(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_02306f08 *w;
    vu16 zero;
    s32 a;
    s32 n;

    task = func_020372d0(func_0206c3d0, func_ov000_02306e90, 0, 0x500, 2, sizeof(UnkStruct_02306f08));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02306f08));
    w->unk2A0 |= 1;
    func_ov000_022fba50(w, (s32)p, b, c);
    switch (p->unk2) {
    case 0x63:
    default:
        a = 0x10;
        n = 8;
        break;
    case 0x64:
        a = 0x10;
        n = 8;
        break;
    case 0x65:
    case 0x66:
        a = 0x12;
        n = 0xc;
        break;
    }
    func_02069f1c(w, data_ov000_023668d0, a, data_ov000_02369ec4, data_ov000_02369ba0);
    func_0206a054(w, 0, 8);
    w->base.spr0.unk46 = 9;
    w->base.spr1.unk46 = 9;
    w->base.spr0.unk2C &= ~0x60000;
    w->base.spr1.unk2C &= ~0x60000;
    func_0203363c(&w->sprA, *w->base.unk8C, 0, 0x40000, 0, 0, func_02039c18(0, n), 0, 0x05000200);
    func_0203363c(&w->sprB, *w->base.unk8C, 0, 0x40000, 1, 0, func_02039c18(1, n), 0, 0x05000600);
    w->sprA.unk46 = w->sprB.unk46 = 0x14;
    w->sprA.unk44 = w->base.spr0.unk44;
    w->sprB.unk44 = w->base.spr1.unk44;
    w->sprA.unk2C |= 0x10;
    w->sprB.unk2C |= 0x10;
    func_0206e2b4(&w->base.coll0, 0, -16);
    w->base.coll0.unk1C = func_ov000_02306d88;
    w->base.coll0.unkA |= 0x800;
    w->base.coll1.unk28 = 0;
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 4;
    switch (w->base.unk1E0->unk2) {
    case 0x63:
        func_02069818(w, 2);
        func_02033550(&w->sprA, 3);
        func_02033550(&w->sprB, 3);
        if (!(w->base.unk1E0->unk4 & 1)) {
            w->unk2A4 = 6;
        } else {
            w->base.unk5C |= 2;
            w->sprA.unk2C |= 0x100;
            w->sprB.unk2C |= 0x100;
            w->unk2A4 = 2;
        }
        break;
    case 0x64:
        func_02069818(w, 0);
        func_02033550(&w->sprA, 1);
        func_02033550(&w->sprB, 1);
        if (!(w->base.unk1E0->unk4 & 1)) {
            w->base.unk5C |= 1;
            w->sprA.unk2C |= 0x80;
            w->sprB.unk2C |= 0x80;
            w->unk2A4 = 0;
        } else {
            w->unk2A4 = 4;
        }
        break;
    case 0x65:
        func_02069818(w, 4);
        func_02033550(&w->sprA, 5);
        func_02033550(&w->sprB, 5);
        if (w->base.unk1E0->unk4 & 1) {
            w->base.unk5C &= ~1;
            w->base.unk5C |= 2;
            w->sprA.unk2C |= 0x100;
            w->sprB.unk2C |= 0x100;
            w->unk2A4 = 1;
        } else {
            w->base.unk5C |= 1;
            w->sprA.unk2C |= 0x80;
            w->sprB.unk2C |= 0x80;
            w->unk2A4 = 5;
        }
        break;
    case 0x66:
        func_02069818(w, 4);
        func_02033550(&w->sprA, 5);
        func_02033550(&w->sprB, 5);
        if (w->base.unk1E0->unk4 & 1) {
            w->base.unk5C |= 3;
            w->sprA.unk2C |= 0x180;
            w->sprB.unk2C |= 0x180;
            w->unk2A4 = 3;
        } else {
            w->base.unk5C &= ~1;
            w->unk2A4 = 7;
        }
        break;
    }
    func_ov000_02306e74(w);
    return w;
}
