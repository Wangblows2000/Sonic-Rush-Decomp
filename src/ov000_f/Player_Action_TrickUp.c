#include <nitro/types.h>

typedef struct UnkStruct_02349e24 UnkStruct_02349e24;

struct UnkStruct_02349e24 {
    u8 pad0[8];
    u32 unk8;
    u8 padC[0x2E - 0xC];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x62 - 0x5E];
    s16 unk62;
    u8 pad64[0x68 - 0x64];
    void (*unk68)(UnkStruct_02349e24 *w);
    u8 pad6C[0x2CC - 0x6C];
    u8 unk2CC[0x324 - 0x2CC];
    u8 unk324;
    u8 pad325[0x32A - 0x325];
    s16 unk32A;
    u32 unk32C;
    u8 pad330[0x352 - 0x330];
    s16 unk352;
    u8 pad354[0x368 - 0x354];
    s16 unk368;
    u8 pad36A[0x370 - 0x36A];
    s16 unk370;
    u8 pad372[0x384 - 0x372];
    u8 unk384[0x3CF - 0x384];
    u8 unk3CF;
    u8 pad3D0;
    u8 unk3D1;
    u8 pad3D2[0x3E2 - 0x3D2];
    s16 unk3E2;
    u8 pad3E4[0x420 - 0x3E4];
    u16 unk420;
};

extern u32 gRandSeed;

BOOL ScorePopup_IsActive(void);
void Player_SetAnimation(UnkStruct_02349e24 *w, s32 state);
void Player_State_InAir(UnkStruct_02349e24 *w);
void EffBoostJet_Spawn(UnkStruct_02349e24 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void ObjRect_SetAtkDefFlags(void *p, s32 a, s32 b);
void ScreenShake_Start(s32 a);
void Player_AddTrickScore(UnkStruct_02349e24 *w, s32 a);

static inline u16 Rand_02349e24(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void Player_Action_TrickUp(UnkStruct_02349e24 *w) {
    s32 v;
    u16 f;

    if (!ScorePopup_IsActive()) {
        w->unk3CF = 0;
    }
    Player_SetAnimation(w, 0x35);
    w->unk58 |= 0x10;
    w->unk58 |= 0x8000;
    w->unk32C |= 0x8000;
    w->unk68 = Player_State_InAir;
    w->unk30 = -w->unk370;
    if (w->unk324 == 1) {
        ObjRect_SetAtkDefFlags(w->unk2CC, 0x13, 0);
        w->unk3E2 = 0;
        w->unk3D1 = 6;
        w->unk30 <<= 1;
        ScreenShake_Start(2);
        w->unk62 = 0x44;
        EffBoostJet_Spawn(w);
        if (w->unk420 & 0x10) {
            v = w->unk2E;
            if (v < 0) {
                v = -v;
            }
            w->unk2E = v;
            w->unk34 = w->unk2E;
        } else if (w->unk420 & 0x20) {
            v = w->unk2E;
            if (v < 0) {
                v = -v;
            }
            w->unk2E = -v;
            w->unk34 = w->unk2E;
        } else {
            w->unk2E = 0;
            w->unk34 = 0;
        }
        Sound_PlaySeqArcEx(w->unk384, 0, 0x1b);
        Sound_PlaySeqArcEx(w->unk384, 0, (Rand_02349e24() & 3) + 0x26);
    } else {
        f = w->unk420;
        if (!(f & 0x30) || (w->unk2E > 0 && (f & 0x20)) || (w->unk2E < 0 && (f & 0x10))) {
            w->unk2E = 0;
            w->unk34 = 0;
        }
        Sound_PlaySeqArcEx(w->unk384, 0, 0xf);
        Sound_PlaySeqArcEx(w->unk384, 0, (Rand_02349e24() & 3) + 0x1d);
    }
    w->unk32C &= ~5;
    w->unk32C |= 1;
    w->unk32C |= 8;
    w->unk8 = 0;
    w->unk32A = 0;
    Player_AddTrickScore(w, 200);
}
