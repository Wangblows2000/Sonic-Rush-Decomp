#include <nitro/types.h>

typedef struct UnkStruct_022cac48_3 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x54 - 0x14];
    u32 unk54;
    u8 pad1[0x33c - 0x58];
    s32 unk33C;
} UnkStruct_022cac48_3;

typedef struct UnkStruct_022cac48_4 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x216 - 0x58];
    u16 unk216;
    u8 pad2[0x242 - 0x218];
    u16 unk242;
    u8 pad3[0x29a - 0x244];
    u16 unk29A;
    u8 pad4[0x420 - 0x29c];
    s32 unk420;
} UnkStruct_022cac48_4;

typedef struct UnkStruct_022cac48 UnkStruct_022cac48;
struct UnkStruct_022cac48 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad0[0x68 - 0x58];
    void (*func)(UnkStruct_022cac48 *s);
    u8 pad1[0x210 - 0x6c];
    UnkStruct_022cac48_3 *unk210;
    UnkStruct_022cac48_4 *unk214;
    u8 pad3[0x350 - 0x218];
    s32 unk350;
};

typedef struct UnkStruct_022cac48_pl {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x3b2 - 0x18];
    u16 unk3B2;
} UnkStruct_022cac48_pl;

typedef struct UnkStruct_022cac48_5 {
    s32 unk0;
    u8 pad[0x18 - 4];
    u32 unk18;
} UnkStruct_022cac48_5;

typedef struct UnkStruct_022cac48_6 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022cac48_6;

extern UnkStruct_022cac48_6 gBossStatus;
extern UnkStruct_022cac48_5 gGameState;
extern UnkStruct_022cac48_pl *gPlayers;

void ScreenShake_Start(s32 a);
void StageObj_SetPause(void);
void func_ov000_022cc35c(s32 a, s32 b);
void Player_Action_Blank(UnkStruct_022cac48_pl *p);
void Player_SetAnimation(UnkStruct_022cac48_pl *p, s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void NNS_SndPlayerStopSeqAll(s32 a);
void Boss1_State_Defeat(UnkStruct_022cac48 *s);

void Boss1_Action_Defeat(UnkStruct_022cac48 *s) {
    UnkStruct_022cac48_3 *p = s->unk210;
    gBossStatus.unk4 = 6;
    s->unk214->unk216 &= ~4;
    s->unk214->unk29A &= ~4;
    s->unk214->unk242 &= ~4;
    ScreenShake_Start(0);
    s->unk54 |= 0x20;
    s->unk210->unk54 |= 0x20;
    s->unk214->unk54 |= 0x20;
    s->unk350 = 0;
    s->unk210->unk33C = 0;
    s->unk214->unk420 = 0;
    StageObj_SetPause();
    gGameState.unk18 &= ~1;
    p->unk10 = 0xe100;
    func_ov000_022cc35c(gGameState.unk0, 0);
    Player_Action_Blank(gPlayers);
    Player_SetAnimation(gPlayers, 0x30);
    gPlayers->unk3B2 = 0;
    gPlayers->unk10 = 0xe100;
    gPlayers->unk14 = 0x1a800;
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    NNS_SndPlayerStopSeqAll(0);
    s->func = Boss1_State_Defeat;
}
