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

extern void *gZoneActArchive;
extern u8 data_ov000_0236604c[];

void BossFPartDD_Destructor(void);
void BossFPartDD_State_HitThenVanish(void);
void BossFPartDD_Draw(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
s32 NNS_G3dGetTex(void *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

UnkStruct_022f9580 *BossFPartDD_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022f9580 *w;
    UnkStruct_Coll *coll;
    UnkStruct_022f9580_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, BossFPartDD_Destructor, 0, 0x501, 2, sizeof(UnkStruct_022f9580))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f9580));
    w->base.unk68 = BossFPartDD_State_HitThenVanish;
    NNS_FndMountArchive(ctx, data_ov000_0236604c, gZoneActArchive);
    w->unk20C = NNS_FndGetArchiveFileByIndex(ctx, 0x6f);
    w->unk210 = NNS_FndGetArchiveFileByIndex(ctx, 0x70);
    w->unk214 = NNS_FndGetArchiveFileByIndex(ctx, 0x71);
    NNS_FndUnmountArchive(ctx);
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = BossFPartDD_Draw;
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
    ObjRect_SetBox2D(coll, -20, -20, 20, 0);
    coll->unk18 = w;
    coll->unkA &= ~4;
    sub = &w->unk218;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk20C, 0);
    Model_SetAnimation(sub, 1, w->unk210, 0, 0);
    Model_SetAnimation(sub, 2, w->unk214, 0, NNS_G3dGetTex(w->unk20C));
    sub->unk114 = 0x1800;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    sub->unkA = 0x13;
    sub->unkB = 3;
    sub->unkC = 0;
    Sound_PlaySeqArc(0, 0, 0xe5);
    return w;
}
