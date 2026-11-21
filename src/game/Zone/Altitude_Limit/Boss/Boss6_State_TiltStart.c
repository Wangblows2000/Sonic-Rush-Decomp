#include <game/Stage/stageobj.h>

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
    u8 padD0C[0xE72 - 0xD0C];
    u16 unkE72;
    u8 padE74[0xF50 - 0xE74];
    s32 unkF50;
    u8 padF54[0xF6A - 0xF54];
    u16 unkF6A;
    u16 unkF6C;
    u8 padF6E[0x1068 - 0xF6E];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
    u8 pad1074[0x1118 - 0x1074];
    s32 unk1118;
    s32 unk111C;
    s32 unk1120;
    s32 unk1124;
    s32 unk1128;
    s32 unk112C;
} UnkStruct_Boss;

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
void Boss6_State_Tilting(void);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

void Boss6_State_TiltStart(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 k;

    k = w->unkF50 << 12;
    data_ov000_0236a444.unk3C = FX_Div(k * ScreenShake_GetOffsetX(), 0x5a000);
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C * 0x3000;
    data_ov000_0236a444.unk3C = data_ov000_0236a444.unk3C >> 12;
    data_ov000_0236a444.unk40 = FX_Div(k * ScreenShake_GetOffsetY(), 0x5a000);
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk40 * 0x3000;
    data_ov000_0236a444.unk40 = data_ov000_0236a444.unk3C >> 12;
    w->unkF50--;
    if (w->unkF50 < 0)
        w->unkF50 = 0;
    if (!(m->unk10C & 2))
        return;
    w->base.unk68 = Boss6_State_Tilting;
    ScreenShake_Start(0);
    Model_SetAnimation(m, 0, w->unkD08, 0xa, 0);
    m->unk114 = 0x1000;
    w->unkF6A = 0x169;
    w->unkF6C = 0x549f;
    w->unkF50 = 0x12c;
    w->unkE72 &= ~4;
    w->unk1124 = 0;
    w->unk1128 = 0;
    w->unk111C = 0;
    w->unk1120 = 0;
    w->unk112C = 10;
    Sound_PlaySeqArc(0, 0, 0x87);
    Sound_PlayVoice(0, 0, 0xd2);
}
