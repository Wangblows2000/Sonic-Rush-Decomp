#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x138 - 0x54];
} UnkStruct_022ee654_Sub;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_022ee654_Sub unk20C;
    u8 pad344[0x378 - 0x344];
    s32 unk378;
    u16 unk37C;
    u8 pad37E[0x384 - 0x37E];
    s32 unk384;
    void *unk388;
    void *unk38C;
    void *unk390;
    void *unk394;
    void *unk398;
    void *unk39C;
    void *unk3A0;
} UnkStruct_022ee654;

typedef struct {
    u8 pad0[0x3D0];
    s32 unk3D0;
    u8 pad3D4[0x42E - 0x3D4];
    u16 unk42E;
} UnkStruct_022ee654_Par;

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    u8 pad1C[0x214 - 0x1C];
    UnkStruct_022ee654_Par *unk214;
} UnkStruct_02369b78;

extern UnkStruct_02369b78 *gBoss7;
extern void *gZoneActArchive;
extern u8 data_ov000_02365ae8[];

void Boss7PartD6_Destructor(void);
void Boss7PartD6_State_Appear(void);
void Boss7PartD6_Draw(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);

UnkStruct_022ee654 *Boss7PartD6_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_02369b78 *mgr = gBoss7;
    UnkStruct_022ee654 *w;
    UnkStruct_022ee654_Par *par = mgr->unk214;
    UnkStruct_Coll *coll;
    UnkStruct_022ee654_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, Boss7PartD6_Destructor, 0, 0x502, 2, sizeof(UnkStruct_022ee654))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022ee654));
    w->base.unk68 = Boss7PartD6_State_Appear;
    w->unk378 = par->unk3D0;
    w->unk37C = par->unk42E;
    w->unk384 = Sound_AllocHandle();
    NNS_FndMountArchive(ctx, data_ov000_02365ae8, gZoneActArchive);
    if (w->unk378 == 0) {
        w->unk388 = NNS_FndGetArchiveFileByIndex(ctx, 0x12);
        w->unk38C = NNS_FndGetArchiveFileByIndex(ctx, 0x13);
        w->unk390 = NNS_FndGetArchiveFileByIndex(ctx, 0x15);
        w->unk394 = NNS_FndGetArchiveFileByIndex(ctx, 0x17);
        w->unk398 = NNS_FndGetArchiveFileByIndex(ctx, 0x14);
        w->unk39C = NNS_FndGetArchiveFileByIndex(ctx, 0x16);
        w->unk3A0 = NNS_FndGetArchiveFileByIndex(ctx, 0x18);
    } else {
        w->unk388 = NNS_FndGetArchiveFileByIndex(ctx, 0x19);
        w->unk38C = NNS_FndGetArchiveFileByIndex(ctx, 0x1a);
        w->unk390 = NNS_FndGetArchiveFileByIndex(ctx, 0x1c);
        w->unk394 = NNS_FndGetArchiveFileByIndex(ctx, 0x1e);
        w->unk398 = NNS_FndGetArchiveFileByIndex(ctx, 0x1b);
        w->unk39C = NNS_FndGetArchiveFileByIndex(ctx, 0x1d);
        w->unk3A0 = NNS_FndGetArchiveFileByIndex(ctx, 0x1f);
    }
    NNS_FndUnmountArchive(ctx);
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = Boss7PartD6_Draw;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x3300;
    w->base.unk10 = b;
    w->base.unk14 = c;
    w->base.unk18 = mgr->unk18;
    if (w->unk378 == 0) {
        coll = &w->base.coll0;
        coll->unkA &= ~4;
        coll = &w->base.coll1;
        ObjRect_SetBox2D(coll, -12, -256, 12, 0);
        coll->unk18 = w;
        coll->unkA &= ~4;
    } else {
        coll = &w->base.coll0;
        coll->unkA &= ~4;
        coll = &w->base.coll1;
        ObjRect_SetBox2D(coll, -8, -256, 8, 0);
        coll->unk18 = w;
        coll->unkA &= ~4;
    }
    sub = &w->unk20C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk388, 0);
    sub->unk48 = w->base.unk10 << 4;
    sub->unk4C = -(w->base.unk14 << 4);
    sub->unk50 = w->base.unk18 << 4;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    return w;
}
