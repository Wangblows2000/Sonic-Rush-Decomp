#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    u8 pad30[0x138 - 0x30];
} UnkStruct_022f9e14_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x20E - 0x20C];
    u16 unk20E;
    s32 unk210;
    s32 unk214;
    s32 unk218;
    u8 pad21C[4];
    void *unk220;
    void *unk224;
    UnkStruct_022f9e14_Sub unk228;
} UnkStruct_022f9e14;

typedef struct {
    u8 pad0[0x340];
    UnkStruct_022f9e14_Sub unk340;
} UnkStruct_022f9e14_Ref;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_022f9e14_Ref *unk218;
} UnkStruct_02369b84;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

extern UnkStruct_02369b84 *gBossF;
extern void *data_ov000_02369b88[];
extern void *gZoneActArchive;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern s16 data_ov000_02363fbc[][8];
extern u8 data_ov000_0236605c[];

void BossFPartD9_Destructor(void);
void BossFPartD9_State_Unk_ov000_022f7f80(UnkStruct_022f9e14 *w);
void BossFPartD9_Draw(void);
void BossFPartD9_OnHit(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

UnkStruct_022f9e14 *BossFPartD9_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022f9e14 *w;
    UnkStruct_022f9e14_Sub *r = &gBossF->unk218->unk340;
    UnkStruct_Coll *coll;
    UnkStruct_022f9e14_Sub *sub;
    vu16 zero;
    s16 v;

    w = Task_Create(StageObj_Main, BossFPartD9_Destructor, 0, 0x501, 2, sizeof(UnkStruct_022f9e14))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f9e14));
    w->base.unk68 = BossFPartD9_State_Unk_ov000_022f7f80;
    w->unk20E = 0xe000;
    w->unk210 = r->unk24 - 0x30000;
    w->unk214 = r->unk28 + 0x18000;
    w->unk218 = r->unk2C + 0x38000;
    NNS_FndMountArchive(ctx, data_ov000_0236605c, gZoneActArchive);
    w->unk224 = NNS_FndGetArchiveFileByIndex(ctx, 8);
    NNS_FndUnmountArchive(ctx);
    w->unk220 = data_ov000_02369b88[2];
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = BossFPartD9_Draw;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0xa000;
    w->base.unk58 |= 0x200;
    v = data_ov000_02363fbc[GetLevel()][gBossStatus.unk8 - 1];
    v += (Rand() & 7) << 4;
    if (p->unk4 & 1) {
        w->base.unk5C |= 1;
        w->base.unk2E = -v;
    } else {
        w->base.unk5C &= ~1;
        w->base.unk2E = v;
    }
    w->base.unk10 = w->unk210 >> 4;
    w->base.unk14 = -w->unk214 >> 4;
    w->base.unk18 = w->unk218 >> 4;
    w->base.unk64 = -16;
    w->base.unk65 = -16;
    w->base.unk66 = 16;
    w->base.unk67 = 16;
    coll = &w->base.coll0;
    ObjRect_SetBox2D(coll, -16, -16, 16, 16);
    ObjRect_SetAtkDefFlags(coll, 0x11, 0);
    coll->unk18 = w;
    coll->unk1C = BossFPartD9_OnHit;
    coll->unkA |= 4;
    coll = &w->base.coll1;
    ObjRect_SetBox2D(coll, -16, -16, 16, 16);
    coll->unkA &= ~4;
    sub = &w->unk228;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk220, 0);
    Model_SetAnimation(sub, 0, w->unk224, 3, 0);
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    ((void (*)(UnkStruct_022f9e14 *))w->base.unk68)(w);
    return w;
}
