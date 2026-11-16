#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x138 - 0x24];
} UnkStruct_022e6500_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022e6500_Sub unk20C;
    void *unk344;
    s32 unk348;
    s32 unk34C;
    s32 unk350;
    s32 unk354;
    u8 pad358[0x360 - 0x358];
    s32 unk360;
    u8 pad364[0x36C - 0x364];
} UnkStruct_022e6500;

typedef struct {
    u8 pad0[0xD5E];
    u16 unkD5E;
} UnkStruct_02369b4c;

extern UnkStruct_02369b4c *data_ov000_02369b4c;
extern void *data_ov000_02369b50[];
extern void *data_022c4488;
extern u8 data_ov000_02365964[];

void func_ov000_022e63ec(void);
void func_ov000_022e162c(void);
void func_ov000_022e5408(void);
void func_ov000_022e0860(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02031c1c(void *s, s32 a);
void func_02031bd0(void *s, void *res, s32 idx);

UnkStruct_022e6500 *func_ov000_022e6500(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022e6500 *w;
    UnkStruct_022e6500_Sub *sub;
    vu16 zero;

    w = func_020372d0(func_0206c3d0, func_ov000_022e63ec, 0, 0x502, 2, sizeof(UnkStruct_022e6500))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022e6500));
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022e162c;
    w->base.unk68 = func_ov000_022e5408;
    w->base.unk5C |= 0x20;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0xa300;
    func_02021310(ctx, data_ov000_02365964, data_022c4488);
    w->unk344 = func_02021264(ctx, 8);
    func_020212e0(ctx);
    sub = &w->unk20C;
    func_02031c1c(sub, 0);
    func_02031bd0(sub, data_ov000_02369b50[9], 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    w->base.coll0.unk18 = w;
    func_0206e2b4(&w->base.coll0, 0x13, -1);
    w->base.coll0.unkA &= ~4;
    w->base.coll0.unkA |= 0x28;
    w->base.coll0.unk1C = func_ov000_022e0860;
    w->base.coll1 = w->base.coll0;
    w->base.coll2.unkA |= 0x1000;
    if (data_ov000_02369b4c->unkD5E != 0) {
        w->unk348 = -0x8000;
        w->unk34C = -0x1000;
        w->unk350 = -0xf000;
        w->unk360 = 0x17800;
    } else {
        w->unk348 = 0x8000;
        w->unk34C = 0x1000;
        w->unk350 = 0xf000;
        w->unk360 = -0x17800;
    }
    w->unk354 = 0x36db;
    return w;
}
