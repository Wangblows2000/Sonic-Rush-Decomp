#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x10C - 0x24];
    u8 unk10C;
    u8 pad10D[0x138 - 0x10D];
} UnkStruct_022f9c38_Sub;

typedef struct {
    UnkStruct_StageObj base;
    void *unk20C;
    void *unk210;
    UnkStruct_022f9c38_Sub unk214;
} UnkStruct_022f9c38;

typedef struct {
    u8 pad0[0x458];
    s32 unk458;
} UnkStruct_022f9c38_Ref;

typedef struct {
    u8 pad0[0x218];
    UnkStruct_022f9c38_Ref *unk218;
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
extern u8 data_ov000_02366058[];

void BossFPartDA_Destructor(void);
void BossFPartDA_State_Unk_ov000_022f7ba8(void);
void BossFPartDA_Draw(void);
void func_02021310(void *f, void *path, void *arc);
void *func_02021264(void *f, s32 i);
void func_020212e0(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);

static inline BOOL IsMode23(void) {
    return gGameState.unk10 == 2 || gGameState.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : gGameState.unk14;
}

UnkStruct_022f9c38 *BossFPartDA_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    UnkStruct_022f9c38 *w;
    UnkStruct_Coll *coll;
    UnkStruct_022f9c38_Sub *sub;
    vu16 zero;

    w = Task_Create(StageObj_Main, BossFPartDA_Destructor, 0, 0x501, 2, sizeof(UnkStruct_022f9c38))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022f9c38));
    w->base.unk68 = BossFPartDA_State_Unk_ov000_022f7ba8;
    func_02021310(ctx, data_ov000_02366058, gZoneActArchive);
    w->unk210 = func_02021264(ctx, 8);
    func_020212e0(ctx);
    w->unk20C = data_ov000_02369b88[3];
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = BossFPartDA_Draw;
    w->base.unk54 |= 0x10;
    w->base.unk58 |= 0x1300;
    if (p->unk4 & 1) {
        w->base.unk5C |= 1;
        w->base.unk2E = -0x300;
    } else {
        w->base.unk5C &= ~1;
        w->base.unk2E = 0x300;
    }
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
    Model_SetAnimation(sub, 0, w->unk210, 4, 0);
    sub->unk10C |= 4;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    return w;
}
