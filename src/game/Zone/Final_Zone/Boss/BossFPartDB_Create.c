#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x10C - 0x24];
    u8 unk10C;
    u8 pad10D[0x138 - 0x10D];
} UnkStruct_022f9988_Sub;

typedef struct {
    UnkStruct_StageObj base;
    void *unk20C;
    void *unk210;
    UnkStruct_022f9988_Sub unk214;
} UnkStruct_022f9988;

typedef struct {
    u8 pad0[0x458];
    s32 unk458;
} UnkStruct_022f9988_Ref;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_022f9988_Ref *unk218;
} UnkStruct_02369b84;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

extern UnkStruct_02369b84 *gBossF;
extern void *data_ov000_02369b88[];
extern void *gZoneActArchive;
extern UnkStruct_022c4560 gGameState;
extern s16 data_ov000_02363e60[][3];
extern s16 data_ov000_02364044[][9];
extern u8 data_ov000_02366054[];

void BossFPartDB_Destructor(void);
void BossFPartDB_State_Move(void);
void BossFPartDB_Draw(void);
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

UnkStruct_022f9988 *BossFPartDB_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_02369b84 *boss = gBossF;
    UnkStruct_022f9988 *w;
    UnkStruct_Coll *coll;
    UnkStruct_022f9988_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, BossFPartDB_Destructor, 0, 0x501, 2, sizeof(UnkStruct_022f9988))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f9988));
    w->base.unk68 = BossFPartDB_State_Move;
    NNS_FndMountArchive(ctx, data_ov000_02366054, gZoneActArchive);
    w->unk210 = NNS_FndGetArchiveFileByIndex(ctx, 8);
    NNS_FndUnmountArchive(ctx);
    w->unk20C = data_ov000_02369b88[4];
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = BossFPartDB_Draw;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x1300;
    if (p->unk4 & 2)
        w->base.unk2E = data_ov000_02363e60[GetLevel()][boss->unk218->unk458];
    else
        w->base.unk2E = data_ov000_02364044[GetLevel()][boss->unk218->unk458];
    if (p->unk4 & 1) {
        w->base.unk5C |= 1;
        w->base.unk2E = -w->base.unk2E;
    } else {
        w->base.unk5C &= ~1;
        w->base.unk2E = w->base.unk2E * 1;
    }
    if (p->unk4 & 4)
        w->base.unk2E >>= 1;
    w->base.unk10 = b;
    w->base.unk14 = c;
    w->base.unk18 = 0;
    coll = &w->base.coll0;
    coll->unkA &= ~4;
    coll = &w->base.coll1;
    ObjRect_SetBox2D(coll, 0x10, -0x20, 0x40, 0);
    coll->unk18 = w;
    coll->unkA |= 4;
    sub = &w->unk214;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk20C, 0);
    Model_SetAnimation(sub, 0, w->unk210, 5, 0);
    sub->unk10C |= 4;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    return w;
}
