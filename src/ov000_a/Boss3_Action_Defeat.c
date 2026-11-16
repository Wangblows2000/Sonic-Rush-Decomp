#include <nitro/types.h>

typedef struct UnkStruct_022d9ab0_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d9ab0_a;

typedef struct UnkStruct_022d9ab0_brl {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x30 - 0x14];
    s16 unk30;
    u8 pad1[0x54 - 0x32];
    u32 unk54;
    u8 pad2[0x5c - 0x58];
    u16 unk5C;
    u8 pad3[0x13e - 0x5e];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x2c4 - 0x16c];
    UnkStruct_022d9ab0_a unk2C4;
    void *unk434;
} UnkStruct_022d9ab0_brl;

typedef struct UnkStruct_022d9ab0_rock {
    u8 pad[0x10];
    s32 unk10;
    u8 pad0[0x2e - 0x14];
    s16 unk2E;
    u8 pad1[0x54 - 0x30];
    u32 unk54;
    u8 pad3[0x13e - 0x58];
    u16 unk13E;
    u8 pad4[0x16a - 0x140];
    u16 unk16A;
    u8 pad5[0x3a4 - 0x16c];
    s32 unk3A4;
    u8 pad6[0x518 - 0x3a8];
    s32 unk518;
} UnkStruct_022d9ab0_rock;

typedef struct UnkStruct_022d9ab0 UnkStruct_022d9ab0;
struct UnkStruct_022d9ab0 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x68 - 0x58];
    void (*func)(UnkStruct_022d9ab0 *s);
    u8 pad2[0x214 - 0x6c];
    UnkStruct_022d9ab0_brl *unk214;
    UnkStruct_022d9ab0_rock *unk218;
    u8 pad4[0x350 - 0x21c];
    s32 unk350;
};

typedef struct UnkStruct_022d9ab0_p {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x32c - 0x5e];
    u32 unk32C;
    u8 pad2[0x3b2 - 0x330];
    s16 unk3B2;
    u8 pad3[0x420 - 0x3b4];
    u16 unk420;
    u16 unk422;
    u16 unk424;
} UnkStruct_022d9ab0_p;

typedef struct UnkStruct_022d9ab0_7 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_022d9ab0_7;

typedef struct UnkStruct_022d9ab0_4 {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022d9ab0_4;

extern UnkStruct_022d9ab0_7 gBossStatus;
extern UnkStruct_022d9ab0_4 gGameState;
extern UnkStruct_022d9ab0_p *gPlayers;

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);
void StageObj_SetPause(void);
void Player_Action_Blank(UnkStruct_022d9ab0_p *p);
u32 RingManager_Unk_ov000_0235b0a8(void);
void RingManager_Unk_ov000_0235b08c(u32 a);
void NNS_SndPlayerStopSeqAll(s32 a);
void Boss3_State_Unk_ov000_022d9aa0(UnkStruct_022d9ab0 *s);

void Boss3_Action_Defeat(UnkStruct_022d9ab0 *s) {
    UnkStruct_022d9ab0_brl *a = s->unk214;
    UnkStruct_022d9ab0_rock *b = s->unk218;
    gBossStatus.unk4 = 0x13;
    a->unk13E &= ~4;
    a->unk16A &= ~4;
    b->unk13E &= ~4;
    b->unk16A &= ~4;
    s->unk54 |= 0x20;
    a->unk54 |= 0x20;
    b->unk54 |= 0x20;
    s->unk350 = 0;
    a->unk2C4.unk114 = 0;
    b->unk3A4 = 0;
    b->unk518 = 0;
    StageObj_SetPause();
    gGameState.unk18 &= ~1;
    Model_SetAnimation(&a->unk2C4, 0, a->unk434, 1, 0);
    a->unk2C4.unk10C |= 4;
    a->unk10 = 0;
    a->unk5C |= 1;
    a->unk30 = 0;
    b->unk10 = -0x4600;
    b->unk2E = 0;
    Player_Action_Blank(gPlayers);
    gPlayers->unk3B2 = 0;
    gPlayers->unk5C |= 0x20;
    gPlayers->unk32C |= 0x200000;
    gPlayers->unk424 = 0;
    gPlayers->unk422 = gPlayers->unk424;
    gPlayers->unk420 = gPlayers->unk422;
    RingManager_Unk_ov000_0235b08c(RingManager_Unk_ov000_0235b0a8() | 1);
    NNS_SndPlayerStopSeqAll(0);
    s->func = Boss3_State_Unk_ov000_022d9aa0;
}
