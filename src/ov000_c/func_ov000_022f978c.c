#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x138 - 0x24];
} UnkStruct_022f978c_Sub;

typedef struct {
    UnkStruct_StageObj base;
    s32 unk20C;
    u8 pad210[0x22C - 0x210];
    s32 unk22C;
    s32 unk230;
    s32 unk234;
    void *unk238;
    UnkStruct_022f978c_Sub unk23C;
} UnkStruct_022f978c;

typedef struct {
    u8 pad0[0x20];
    union {
        u16 raw;
        struct {
            u16 b0 : 1;
            u16 b1 : 5;
            u16 b6 : 2;
            u16 b8 : 8;
        } bits;
    } unk20;
    u8 pad22[2];
    struct {
        u16 b0 : 5;
        u16 b5 : 11;
    } unk24;
} UnkStruct_0208922c;

extern void *data_022c4488;
extern UnkStruct_0208922c data_0208922c;
extern u8 data_ov000_02366050[];

void func_ov000_022f7a60(void);
void func_ov000_022f78bc(void);
void func_ov000_022f7180(void);
void func_ov000_022f75d8(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void func_02031c1c(void *s, s32 a);
void func_02031bd0(void *s, void *res, s32 idx);
void func_02036754(s32 a, s32 b, s32 c);
void func_020221fc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

UnkStruct_022f978c *func_ov000_022f978c(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022f978c *w;
    UnkStruct_022f978c_Sub *sub;
    vu16 zero;

    w = func_020372d0(func_0206c3d0, func_ov000_022f7a60, 0, 0x501, 2, sizeof(UnkStruct_022f978c))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f978c));
    func_02021310(ctx, data_ov000_02366050, data_022c4488);
    w->unk238 = func_02021264(ctx, 0x6e);
    func_020212e0(ctx);
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022f78bc;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x3300;
    if (p->unk4 & 1) {
        w->unk20C = 2;
        w->base.unkC = p->unk4 >> 1;
        w->base.unk68 = func_ov000_022f7180;
        func_02036754(0, 0, 0xe0);
    } else {
        w->base.unk68 = func_ov000_022f75d8;
        func_02036754(0, 0, 0xe0);
    }
    w->base.unk10 = b;
    w->base.unk14 = c;
    w->base.unk18 = 0;
    w->unk22C = w->base.unk10;
    w->unk230 = w->base.unk14 + 0x64;
    w->unk234 = w->base.unk18;
    sub = &w->unk23C;
    func_02031c1c(sub, 0);
    func_02031bd0(sub, w->unk238, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    func_020221fc(0, 0, 2, 0, 0x10, 0);
    data_0208922c.unk20.raw = 0;
    data_0208922c.unk20.bits.b0 = 1;
    data_0208922c.unk20.bits.b6 = 2;
    data_0208922c.unk24.b0 = 7;
    return w;
}
