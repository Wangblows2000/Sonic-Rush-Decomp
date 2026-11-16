#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x10E - 0x24];
    u8 unk10E;
    u8 pad10F[0x144 - 0x10F];
} UnkStruct_022f92dc_Sub2;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x138 - 0x24];
} UnkStruct_022f92dc_Sub;

typedef struct {
    UnkStruct_StageObj base;
    s32 unk20C;
    u8 pad210[4];
    s16 unk214;
    u8 pad216[0x224 - 0x216];
    UnkStruct_022f92dc_Sub2 unk224[2];
    u8 pad4AC[0x4C8 - 0x4AC];
    void *unk4C8;
    void *unk4CC;
    void *unk4D0;
    UnkStruct_022f92dc_Sub unk4D4;
} UnkStruct_022f92dc;

typedef struct {
    u8 pad0[0x438];
    u16 unk438;
} UnkStruct_022f92dc_Ref;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_022f92dc_Ref *unk218;
} UnkStruct_02369b84;

extern UnkStruct_02369b84 *data_ov000_02369b84;
extern void *data_ov000_02369b88[];
extern void *gZoneActArchive;
extern u8 data_ov000_02366048[];

void func_ov000_022f703c(void);
void func_ov000_022f6834(void);
void func_ov000_022f5c78(void);
void func_ov000_022f6ddc(void);
void func_ov000_022f6cb0(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
s32 func_02025e8c(void *p);

UnkStruct_022f92dc *func_ov000_022f92dc(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_02369b84 *boss = data_ov000_02369b84;
    UnkStruct_022f92dc *w;
    UnkStruct_Coll *coll;
    UnkStruct_022f92dc_Sub *sub;
    u16 i;
    UnkStruct_022f92dc_Sub2 *s;
    vu16 zero;

    w = Task_Create(StageObj_Main, func_ov000_022f703c, 0, 0x501, 2, sizeof(UnkStruct_022f92dc))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f92dc));
    w->unk20C = p->unk4 & 0xf;
    w->unk214 = p->unk4 >> 4;
    switch (w->unk20C) {
    case 0:
        w->base.unk68 = func_ov000_022f6834;
        break;
    case 1:
        w->base.unk68 = func_ov000_022f5c78;
        break;
    }
    boss->unk218->unk438++;
    func_02021310(ctx, data_ov000_02366048, gZoneActArchive);
    w->unk4CC = func_02021264(ctx, 0x72);
    w->unk4D0 = func_02021264(ctx, 0x73);
    func_020212e0(ctx);
    w->unk4C8 = data_ov000_02369b88[2];
    func_ov000_022fba50(w, (s32)p, b, c);
    w->base.unk70 = func_ov000_022f6ddc;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x8200;
    w->base.unk10 = b;
    w->base.unk14 = c;
    w->base.unk18 = 0;
    coll = &w->base.coll0;
    ObjRect_SetBox2D(coll, -0x400, -0x400, 0x400, 0x400);
    ObjRect_SetAtkDefFlags(coll, 0x11, 0);
    coll->unkA |= 0x40;
    coll->unk1C = func_ov000_022f6cb0;
    coll->unk18 = w;
    coll->unkA &= ~4;
    sub = &w->unk4D4;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk4C8, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    for (i = 0; i < 2; i++) {
        s = &w->unk224[i];
        Model_Init(s, 0);
        Model_SetResource(s, w->unk4CC, 0);
        Model_SetAnimation(s, 2, w->unk4D0, 0, func_02025e8c(w->unk4CC));
        s->unk10E |= 4;
        s->unk18 = s->unk1C = s->unk20 = 0x34cc;
    }
    return w;
}
