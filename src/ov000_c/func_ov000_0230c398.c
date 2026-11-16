#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0xBC - 0xC];
    u32 unkBC;
    u8 padC0[0x104 - 0xC0];
} UnkStruct_0230c398_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_0230c398_Sub sub;
} UnkStruct_0230c398;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
} UnkStruct_022c4534;

extern UnkStruct_022c4534 data_022c4534;
extern s16 data_022c4ef8;
extern void *data_ov000_02369ba0;
extern void *data_022c448c;
extern u8 data_ov000_02369f74[], data_ov000_02366a3c[], data_ov000_02369fac[], data_ov000_02366a54[];
extern u8 data_ov000_02369fbc[], data_ov000_02366a6c[], data_ov000_02369fa4[], data_ov000_02369fdc[];
extern u8 data_ov000_02369f8c[], data_ov000_02366a84[], data_ov000_02369fec[], data_ov000_02366aa0[];
extern u8 data_ov000_02369f84[], data_ov000_02369fcc[], data_ov000_02369f7c[], data_ov000_02366ab4[];
extern u8 data_ov000_02369ffc[];

void func_ov000_022fb9c8(void);
void func_ov000_0230bd78(void);
void func_02069934(void *w, UnkStruct_0230c398_Sub *s, void *a, s32 b, s32 c, void *d, void *e);
void func_02068358(UnkStruct_0230c398_Sub *s, void *a, s32 b, s32 c);
void *func_ov000_0230be44(void);
void *func_ov000_0230c02c(void);
void func_0206e2d8(UnkStruct_Coll *c, s32 a, s32 b, s32 d, s32 e, s32 f, s32 g);
void func_ov000_0230c370(UnkStruct_0230c398 *w);

UnkStruct_0230c398 *func_ov000_0230c398(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0230c398 *w;
    vu16 zero;

    if ((b >> 8) < data_022c4534.unk1C + 0xc8 || data_022c4ef8 == 0)
        return NULL;
    task = func_020372d0(func_0206c3d0, func_ov000_022fb9c8, 0, 0x500, 2, sizeof(UnkStruct_0230c398));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0230c398));
    func_ov000_022fba50(w, (s32)p, b, c);
    switch (w->base.unk1E0->unk2) {
    case 0xb9:
        func_02069934(w, &w->sub, data_ov000_02366a3c, 0, 0, data_ov000_02369f74, data_ov000_02369ba0);
        func_02068358(&w->sub, data_ov000_02369fac, 0x200, 0x10);
        if (!(w->base.unk1E0->unk4 & 1))
            w->sub.unkBC |= 8;
        w->base.unk54 |= 2;
        break;
    case 0xba:
        func_02069934(w, &w->sub, data_ov000_02366a54, 0, 0, data_ov000_02369f74, data_ov000_02369ba0);
        func_02069818(w, 1);
        func_02068358(&w->sub, data_ov000_02369fbc, 0x80, 0x10);
        if (!(w->base.unk1E0->unk4 & 1))
            w->sub.unkBC |= 8;
        w->base.unk54 |= 2;
        break;
    case 0x13:
        func_02069f1c(w, data_ov000_02366a6c, 0, data_ov000_02369fa4, data_ov000_02369ba0);
        func_02069eb0(w, 0xffff, data_ov000_02369fdc);
        func_0206a054(w, 0, 0xe);
        w->base.spr0.unk2C |= 0x80000;
        func_02069934(w, &w->sub, data_ov000_02366a84, 0, 0, data_ov000_02369f8c, data_ov000_02369ba0);
        func_02068358(&w->sub, data_ov000_02369fec, 0x800, 0x10);
        w->base.unk88 = func_ov000_0230be44();
        w->base.spr0.unk2C |= 8;
        w->base.spr1.unk2C |= 8;
        w->sub.unkBC |= 8;
        w->base.unk54 |= 0x200;
        break;
    case 0xb8:
        func_02069f1c(w, data_ov000_02366aa0, 0, data_ov000_02369f84, data_022c448c);
        func_02069eb0(w, 0xffff, data_ov000_02369fcc);
        w->base.spr0.unk2C |= 0x80000;
        func_02069934(w, &w->sub, data_ov000_02366ab4, 0, 0, data_ov000_02369f7c, data_ov000_02369ba0);
        func_02068358(&w->sub, data_ov000_02369ffc, 0x80, 0x10);
        w->base.spr0.unk44 = 2;
        w->base.spr1.unk44 = 2;
        w->base.unk88 = func_ov000_0230c02c();
        w->base.spr0.unk2C |= 8;
        w->base.spr1.unk2C |= 8;
        w->sub.unkBC |= 8;
        w->base.unk54 |= 0x200;
        func_0206e2d8(&w->base.coll0, -6, -6, 6, 6, -10, 10);
        func_0206e2b4(&w->base.coll0, 0, -16);
        w->base.coll0.unk1C = func_ov000_0230bd78;
        w->base.coll0.unk18 = w;
        break;
    }
    w->sub.unkA = 4;
    w->sub.unkB = 0;
    w->base.unk18 = -0xc400;
    w->base.unk58 |= 0x100;
    w->base.unk58 |= 0x8000000;
    if (w->base.unk1E0->unk4 & 1)
        w->base.unk54 |= 0x10;
    func_ov000_0230c370(w);
    return w;
}
