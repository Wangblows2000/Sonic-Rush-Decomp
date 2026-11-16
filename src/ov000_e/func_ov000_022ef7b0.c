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
} UnkStruct_0208922c;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x3b2 - 0x20c];
    u16 unk3B2;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x358 - 0x210];
    s32 unk358;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x584 - 0x20C];
    s32 unk584;
} UnkStruct_022ef7b0;

typedef struct {
    u32 unk0;
    u8 pad4[0x18 - 4];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct {
    u32 unk0;
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_Boss *data_ov000_02369b84;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_Player *gPlayers;
extern UnkStruct_0208922c gDisplayMain;

void func_ov000_022f9044(UnkStruct_022ef7b0 *s);
void func_ov000_022f4cbc(UnkStruct_022ef7b0 *s);
void StageObj_SetPause(void);
void func_ov000_022f8b70(u32 a, s32 b);
void func_ov000_02347c44(UnkStruct_Player *p);
void Player_SetAnimation(UnkStruct_Player *p, s32 a);
u32 func_ov000_0235b0a8(void);
void func_ov000_0235b08c(u32 a);
void Task_DestroyByTag(s32 a);
void NNS_SndPlayerStopSeqAll(s32 a);
void BossFPartD8_State_Unk_ov000_022ef6a0(void);

void func_ov000_022ef7b0(UnkStruct_022ef7b0 *s) {
    UnkStruct_Boss *b = data_ov000_02369b84;

    data_ov000_02369ac0.unk4 = 0x3b;
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->base.unk1B8 = NULL;
    b->base.unk54 |= 0x20;
    s->base.unk54 |= 0x20;
    b->unk358 = 0;
    s->unk584 = 0;
    StageObj_SetPause();
    func_ov000_022f8b70(data_022c4560.unk0, 0);
    data_022c4560.unk18 &= ~1;
    func_ov000_02347c44(gPlayers);
    Player_SetAnimation(gPlayers, 0x30);
    gPlayers->unk3B2 = 0;
    gPlayers->base.unk10 = 0x19e00;
    gPlayers->base.unk14 = 0x2b100;
    gPlayers->base.unk18 = 0x2000;
    s->base.unk18 += 0x1400;
    func_ov000_0235b08c(func_ov000_0235b0a8() | 1);
    data_ov000_02369b84->unk20C &= ~0xe;
    Task_DestroyByTag(4);
    gDisplayMain.unk20.bits.b6 = 0;
    b->unk20C &= ~0x10;
    NNS_SndPlayerStopSeqAll(0);
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022ef6a0;
}
