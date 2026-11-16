#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x138 - 0x24];
} UnkStruct_022ee43c_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022ee43c_Sub unk20C;
    u32 unk344;
    u8 pad348[4];
    void *unk34C;
    void *unk350;
    void *unk354;
    void *unk358;
    void *unk35C;
    void *unk360;
    void *unk364;
} UnkStruct_022ee43c;

extern void *data_022c4488;
extern u8 data_ov000_02365ae4[];

void func_ov000_022ec138(void);
void func_ov000_022ebfac(void);
void func_ov000_022ebfc8(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02031c1c(void *s, s32 a);
void func_02031bd0(void *s, void *res, s32 idx);

UnkStruct_022ee43c *func_ov000_022ee43c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022ee43c *w;
    UnkStruct_022ee43c_Sub *sub;
    vu16 zero;

    w = func_020372d0(func_0206c3d0, func_ov000_022ec138, 0, 0x502, 2, sizeof(UnkStruct_022ee43c))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ee43c));
    w->base.unk68 = func_ov000_022ebfac;
    w->unk344 = p->unk4;
    func_02021310(ctx, data_ov000_02365ae4, data_022c4488);
    if (p->unk4 == 0) {
        w->unk34C = func_02021264(ctx, 0x20);
        w->unk350 = func_02021264(ctx, 0x21);
        w->unk35C = func_02021264(ctx, 0x22);
        w->unk354 = func_02021264(ctx, 0x23);
        w->unk360 = func_02021264(ctx, 0x24);
        w->unk358 = func_02021264(ctx, 0x25);
        w->unk364 = func_02021264(ctx, 0x26);
    } else {
        w->unk34C = func_02021264(ctx, 0x27);
        w->unk350 = func_02021264(ctx, 0x28);
        w->unk35C = func_02021264(ctx, 0x29);
        w->unk354 = func_02021264(ctx, 0x2a);
        w->unk360 = func_02021264(ctx, 0x2b);
        w->unk358 = func_02021264(ctx, 0x2c);
        w->unk364 = func_02021264(ctx, 0x2d);
    }
    func_020212e0(ctx);
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022ebfc8;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x3300;
    w->base.unk5C |= 0x20;
    sub = &w->unk20C;
    func_02031c1c(sub, 0);
    func_02031bd0(sub, w->unk34C, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    return w;
}
