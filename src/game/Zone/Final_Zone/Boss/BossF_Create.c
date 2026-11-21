#include <game/Stage/stageobj.h>

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
} UnkStruct_022fa548_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    u16 unk214;
    u8 pad216[2];
    s32 unk218;
    u8 pad21C[4];
    void *unk220;
    void *unk224;
    void *unk228;
    void *unk22C;
    void *unk230;
    void *unk234;
    void *unk238;
    u8 pad23C[4];
    void *unk240;
    UnkStruct_022fa548_Sub unk244;
    UnkStruct_022fa548_Sub unk37C[4];
    UnkStruct_022fa548_Sub unk85C;
    u8 pad994[0x9A4 - 0x994];
    s32 unk9A4;
    s32 unk9A8;
    s32 unk9AC;
    s32 unk9B0;
    u8 pad9B4[0x9BC - 0x9B4];
    s32 unk9BC;
    s32 unk9C0;
    u8 pad9C4[0x9D0 - 0x9C4];
} UnkStruct_022fa548;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    s32 unk0;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[4];
    s32 unk24;
} UnkStruct_022c4534;

extern UnkStruct_022fa548 *gBossF;
extern UnkStruct_02369ac0 gBossStatus;
extern void *data_ov000_02369b88[];
extern void *gZoneActArchive;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022c4534 gStageMapCollision;
extern u8 data_ov000_02366064[], data_ov000_02366068[], data_ov000_02366178[], data_ov000_0236618c[];
extern u8 data_ov000_0236606c[], data_ov000_02366080[], data_ov000_02366098[], data_ov000_023660b0[], data_ov000_023660c8[], data_ov000_023660e0[], data_ov000_023660f8[], data_ov000_02366110[], data_ov000_02366128[], data_ov000_02366144[], data_ov000_0236615c[];

void BossF_Destructor(void);
void BossF_State_WaitForStart(void);
void BossF_Draw(void);
u16 Boss_GetStartHealth(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_G3dResDefaultSetup(void *p);
void *NNS_FndGetArchiveFileByName(void *a);
void NNS_FndUnmountArchive(void *f);
void Model_Init(void *s, s32 a);
void Model_SetResource(void *s, void *res, s32 idx);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
s32 MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void BossF_LoadModelPalettesAll(s32 a, s32 b);
void *File_Load(void *a, s32 b);

UnkStruct_022fa548 *BossF_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    u32 ctx[0x17];
    u32 ctx2[0x17];
    UnkStruct_022fa548 *w;
    UnkStruct_022fa548_Sub *sub;
    vu16 zero;
    vu16 zero2;
    u32 i;

    if (p->unk0 == 0xff && p->unk1 == 0xff)
        return NULL;
    w = Task_Create(StageObj_Main, BossF_Destructor, 0, 0x500, 2, sizeof(UnkStruct_022fa548))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_022fa548));
    gBossF = w;
    w->base.unk68 = BossF_State_WaitForStart;
    w->unk214 = Rand() & 1;
    w->unk9BC = -0x300000;
    w->unk9C0 = 0xfe978000;
    w->unk9A4 = -0x300000;
    w->unk9A8 = 0xfe978000;
    w->unk9AC = 0xfe978000;
    w->unk9B0 = 0xfd5f0000;
    zero2 = 0;
    MIi_CpuClear16(zero2, &gBossStatus, sizeof(UnkStruct_02369ac0));
    gBossStatus.unk4 = 0x2f;
    gBossStatus.unk8 = Boss_GetStartHealth();
    gBossStatus.unkA = Boss_GetStartHealth();
    NNS_FndMountArchive(ctx, data_ov000_02366064, gZoneActArchive);
    w->unk224 = NNS_FndGetArchiveFileByIndex(ctx, 8);
    w->unk228 = NNS_FndGetArchiveFileByIndex(ctx, 9);
    w->unk22C = NNS_FndGetArchiveFileByIndex(ctx, 10);
    w->unk230 = NNS_FndGetArchiveFileByIndex(ctx, 11);
    NNS_FndUnmountArchive(ctx);
    w->unk220 = data_ov000_02369b88[0];
    w->unk234 = data_ov000_02369b88[5];
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk70 = BossF_Draw;
    w->base.unk54 |= 0x12;
    w->base.unk58 |= 0xa300;
    w->base.unk5C |= 0x20;
    w->base.unk10 = 0x1a000;
    w->base.unk14 = 0x30000;
    w->base.unk18 = 0;
    sub = &w->unk244;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk220, 0);
    Model_SetAnimation(sub, 0, w->unk224, 1, 0);
    Model_SetAnimation(sub, 4, w->unk228, 0, 0);
    sub->unk48 = w->base.unk10 << 4;
    sub->unk4C = -(w->base.unk14 << 4);
    sub->unk50 = w->base.unk18 << 4;
    sub->unk18 = sub->unk1C = sub->unk20 = 0x34cc;
    w->unk218 = MapObject_Spawn(0xd8, 0, 0, 0, 0, 0, 0, 0, 0);
    BossF_LoadModelPalettesAll(gGameState.unk0, 0);
    NNS_FndMountArchive(ctx2, data_ov000_02366068, gZoneActArchive);
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_0236606c));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02366080));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02366098));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023660b0));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023660c8));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023660e0));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_023660f8));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02366110));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02366128));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_02366144));
    NNS_G3dResDefaultSetup(NNS_FndGetArchiveFileByName(data_ov000_0236615c));
    NNS_FndUnmountArchive(ctx2);
    w->unk238 = File_Load(data_ov000_02366178, 0);
    w->unk240 = File_Load(data_ov000_0236618c, 0);
    NNS_G3dResDefaultSetup(w->unk238);
    NNS_G3dResDefaultSetup(w->unk240);
    sub = &w->unk85C;
    Model_Init(sub, 0);
    Model_SetResource(sub, w->unk234, 0);
    sub = w->unk37C;
    for (i = 0; i < 4; i++) {
        Model_Init(sub, 0);
        Model_SetResource(sub, w->unk240, i);
        sub++;
    }
    sub = &w->unk37C[0];
    sub->unk48 = 0xc8000;
    sub->unk50 = -0x3e8000;
    sub->unk4C = -0x3c8000;
    gStageMapCollision.unk1C = 0;
    gStageMapCollision.unk24 = 0x340;
    return w;
}
