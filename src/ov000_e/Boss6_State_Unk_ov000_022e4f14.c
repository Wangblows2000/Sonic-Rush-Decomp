#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x960 - 0x20C];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD5E - 0xD0C];
    u16 unkD5E;
    u8 padD60[0xF50 - 0xD60];
    s32 unkF50;
    u8 padF54[0x1068 - 0xF54];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
    u8 pad1074[0x1118 - 0x1074];
    s16 unk1118;
    u8 pad111A[2];
    s32 unk111C;
    s32 unk1120;
    s32 unk1124;
    s32 unk1128;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x32c - 0x20c];
    u32 unk32C;
} UnkStruct_Player;

extern UnkStruct_Player *gPlayers;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void BossEffect_CreateImpactSmall(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_Action_Unk_ov000_022e450c(UnkStruct_Boss *w);
void Boss6_State_Unk_ov000_022e4da0(void);
extern s32 data_ov000_023637bc[];
void Boss6_Unk_ov000_022e05b0(UnkStruct_Boss *w);
void Sound_PlayVoice(s32 a, s32 b, s32 c);

static inline s32 Mul64(s64 a, s64 b) {
    return (s32)((a * b + 0x800) >> 12);
}

void Boss6_State_Unk_ov000_022e4f14(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;
    s32 x;
    s32 v;
    s32 k;
    s32 cur;
    s32 tgt;
    s32 i;

    w->unk1124--;
    if (w->unk1124 >= 0) {
        x = gPlayers->base.unk10;
        if (x < 0x3000)
            x = 0x3000;
        else if (x > 0x31000)
            x = 0x31000;
        w->unk1120 = x;
    }
    w->unk1118 += 0x11;
    v = w->unk1118;
    if (v < 0)
        v = 0;
    else if (v > 0x1000)
        v = 0x1000;
    w->unk1118 = v;
    k = w->unk1118;
    tgt = w->unk1120;
    cur = w->unk111C;
    i = 2;
    do {
        cur += Mul64(tgt - cur, k);
    } while (i-- != 0);
    w->base.unk10 = cur;
    if (!(m->unk10C & 2))
        return;
    w->base.unk68 = Boss6_State_Unk_ov000_022e4da0;
    Model_SetAnimation(m, 0, w->unkD08, 2, 0);
    m->unk114 = data_ov000_023637bc[Rand() & 3];
}
