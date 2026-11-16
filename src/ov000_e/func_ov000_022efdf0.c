#include <ov000_c/stageobj.h>

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

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x224 - 0x210];
    void *unk224;
    void *unk228;
    u8 pad22C[0x244 - 0x22C];
    u8 unk244[4];
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x438 - 0x20C];
    s32 unk438;
    u16 unk43C;
    u8 pad43E[0x454 - 0x43E];
    u16 unk454;
} UnkStruct_022efdf0;

typedef struct {
    u8 pad[0x8c];
    s32 unk8C;
} UnkStruct_0236a444;

typedef struct {
    u32 unk0;
} UnkStruct_022c4560;

extern UnkStruct_Boss *gBossF;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_0236a444 data_ov000_0236a444;

void BossFPartD8_InitHitboxes(UnkStruct_022efdf0 *s);
void func_ov000_022f4cbc(UnkStruct_022efdf0 *s);
void BossF_LoadPalette(u32 a, s32 b);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void BossEffect_CreateBaseDest(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022efd08(void);

void func_ov000_022efdf0(UnkStruct_022efdf0 *s) {
    UnkStruct_Boss *b = gBossF;

    BossFPartD8_InitHitboxes(s);
    func_ov000_022f4cbc(s);
    BossF_LoadPalette(gGameState.unk0, 0);
    Model_SetAnimation(b->unk244, 0, b->unk224, 2, 0);
    Model_SetAnimation(b->unk244, 4, b->unk228, 1, 0);
    BossEffect_CreateBaseDest(0, 0x1a000, 0x30000, 0);
    s->base.unk32 = 0x800;
    gPlayers->unk54 &= ~1;
    gDisplayMain.unk20.raw = 0;
    gDisplayMain.unk20.bits.b0 = 1;
    gDisplayMain.unk24.b0 = 0x10;
    gDisplayMain.unk20.bits.b6 = 2;
    s->unk438 = gPlayers->unk18;
    s->unk43C = 0x1000;
    b->unk20C &= ~0x10;
    data_ov000_0236a444.unk8C = 0;
    Sound_PlaySeqArc(0, 0, 0xca);
    s->unk454 = 0;
    s->base.unk68 = func_ov000_022efd08;
}
