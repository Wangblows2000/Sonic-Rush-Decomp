#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 padD[0x18 - 0xD];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x114 - 0x24];
    s32 unk114;
    u8 pad118[0x138 - 0x118];
} UnkStruct_022f9580_Sub;

typedef struct {
    UnkStruct_StageObj base;
    void *unk20C;
    void *unk210;
    void *unk214;
    UnkStruct_022f9580_Sub unk218;
} UnkStruct_022f9580;

extern void *data_022c4488;
extern u8 data_ov000_0236604c[];

void func_ov000_022f715c(void);
void func_ov000_022f7080(void);
void func_ov000_022f70f4(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02031c1c(void *s, s32 a);
void func_02031bd0(void *s, void *res, s32 idx);
void func_02031a68(void *s, s32 a, void *b, s32 c, s32 d);
s32 func_02025e8c(void *p);
void func_02036754(s32 a, s32 b, s32 c);

UnkStruct_022f9580 *func_ov000_022f9580(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022f9580 *w;
    UnkStruct_Coll *coll;
    UnkStruct_022f9580_Sub *sub;
    vu16 zero;

    w = func_020372d0(func_0206c3d0, func_ov000_022f715c, 0, 0x501, 2, sizeof(UnkStruct_022f9580))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f9580));
    w->base.unk68 = func_ov000_022f7080;
    func_02021310(ctx, data_ov000_0236604c, data_022c4488);
    w->unk20C = func_02021264(ctx, 0x6f);
    w->unk210 = func_02021264(ctx, 0x70);
    w->unk214 = func_02021264(ctx, 0x71);
    func_020212e0(ctx);
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022f70f4;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x3300;
    w->base.unk5C |= 0x20;
    w->base.unk10 = b;
    w->base.unk14 = c;
    w->base.unk18 = 0;
    w->base.unkC = -p->unk4;
    coll = &w->base.coll0;
    coll->unkA &= ~4;
    coll = &w->base.coll1;
    func_0206e3c0(coll, -20, -20, 20, 0);
    coll->unk18 = w;
    coll->unkA &= ~4;
    sub = &w->unk218;
    func_02031c1c(sub, 0);
    func_02031bd0(sub, w->unk20C, 0);
    func_02031a68(sub, 1, w->unk210, 0, 0);
    func_02031a68(sub, 2, w->unk214, 0, func_02025e8c(w->unk20C));
    sub->unk114 = 0x1800;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unkA = 0x13;
    sub->unkB = 3;
    sub->unkC = 0;
    func_02036754(0, 0, 0xe5);
    return w;
}
