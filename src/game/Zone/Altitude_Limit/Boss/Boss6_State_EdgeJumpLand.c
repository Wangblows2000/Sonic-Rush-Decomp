#include <game/Stage/stageobj.h>

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
    u8 padD0C[0xE72 - 0xD0C];
    u16 unkE72;
    u8 padE74[0xF50 - 0xE74];
    s32 unkF50;
    u8 padF54[0x1068 - 0xF54];
    s32 unk1068;
    u8 pad106C[4];
    s32 unk1070;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x32C - 0x20C];
    u32 unk32C;
} UnkStruct_Player;

typedef struct {
    u8 pad0[0x30];
    u16 unk30;
    u8 pad32[0x6C - 0x32];
    u16 unk6C;
} UnkStruct_Cam;

extern UnkStruct_Player *gPlayers;
extern UnkStruct_Cam data_ov000_0236a48c;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void ScreenShake_Start(s32 a);
void BossEffect_CreateLand(s32 a, s32 b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_State_EdgeJumpRecover(void);

void Boss6_State_EdgeJumpLand(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (m->unk10C & 2) {
        w->base.unk68 = Boss6_State_EdgeJumpRecover;
        Model_SetAnimation(m, 0, w->unkD08, 0xf, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        ScreenShake_Start(0xc);
        w->unkF50 = 0x14;
        BossEffect_CreateLand(1, w->unk1068 >> 4, 0x20000, w->unk1070 >> 4);
        w->unkE72 &= ~4;
        w->unkE72 |= 0x1000;
        Sound_PlaySeqArc(0, 0, 0x86);
    }
    if (gPlayers->unk32C & 0x400) {
        UnkStruct_Cam *c = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
        c->unk30 = 0x10;
        c->unk6C = 0x10;
    }
}
