#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD5E - 0xD0C];
    u16 unkD5E;
    u8 padD60[0xD96 - 0xD60];
    u16 unkD96;
    u8 padD98[0xE1A - 0xD98];
    u16 unkE1A;
    u8 padE1C[0xE46 - 0xE1C];
    u16 unkE46;
    u8 padE48[0xF50 - 0xE48];
    s32 unkF50;
    u8 padF54[0x1068 - 0xF54];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
    u8 pad1074[0x1118 - 0x1074];
    s32 unk1118;
    s32 unk111C;
    s32 unk1120;
} UnkStruct_Boss;

typedef struct {
    u8 pad[0x430];
    u16 unk430;
    u16 unk432;
    u16 unk434;
    u16 unk436;
} UnkStruct_Player;


typedef struct {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_0236a444;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_0236a444 data_ov000_0236a444;
extern s32 data_ov000_02363a18[][8][4];

s32 ScreenShake_GetOffsetX(void);
s32 ScreenShake_GetOffsetY(void);
s32 FX_Div(s32 a, s32 b);
void BossEffect_CreatePick(s32 a, s32 b, s32 c, s32 d);
void BossEffect_CreateParts(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_State_AfterLand(void);

typedef struct {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x6c - 0x32];
    u16 unk6C;
} UnkStruct_0236a48c;

typedef struct {
    u8 pad[0x32c];
    u32 unk32C;
} UnkStruct_Pl2;

extern UnkStruct_0236a48c data_ov000_0236a48c;
extern UnkStruct_Pl2 *gPlayers;
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_Unk_ov000_022e04a8(UnkStruct_Boss *w);

void Boss6_State_LandShake(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 k;

    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    k = w->unkF50 << 12;
    data_ov000_0236a444.unk3C = FX_Div(k * ScreenShake_GetOffsetX(), 0x14000);
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C << 13;
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C >> 12;
    data_ov000_0236a444.unk40 = FX_Div(k * ScreenShake_GetOffsetY(), 0x14000);
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk40 << 13;
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk3C >> 12;
    if (m->unk10C & 2) {
        w->base.unk68 = Boss6_State_AfterLand;
        Model_SetAnimation(m, 0, w->unkD08, 0x15, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        ScreenShake_Start(0);
        w->unkE1A &= ~4;
        w->unkE1A |= 0x1000;
        w->unkD96 &= ~4;
        w->unkD96 |= 0x1000;
        w->unkE46 &= ~4;
        w->unkE46 |= 0x1000;
    }
    if (gPlayers->unk32C & 0x400) {
        UnkStruct_0236a48c *c = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;
        c->unk30 = 0x10;
        c->unk6C = 0x10;
    }
}
