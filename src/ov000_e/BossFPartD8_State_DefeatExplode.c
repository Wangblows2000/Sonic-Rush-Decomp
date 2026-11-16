#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x32c - 0x20c];
    u32 unk32C;
    u8 pad330[0x420 - 0x330];
    u16 unk420;
    u16 unk422;
    u16 unk424;
} UnkStruct_Player;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x358 - 0x20C];
    s32 unk358;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u8 pad436[0x584 - 0x436];
    s32 unk584;
} UnkStruct_022ef360;

typedef struct {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 pad60[0x6c - 0x60];
    u16 unk6C;
    u8 pad6E[0x78 - 0x6e];
    s32 unk78;
    u8 pad7C[0x80 - 0x7c];
    u16 unk80;
    u8 pad82[0x84 - 0x82];
    s32 unk84;
    u8 pad88[0x8c - 0x88];
    u16 unk8C;
    u16 unk8E;
    u8 pad90[0x92 - 0x90];
    u16 unk92;
} UnkStruct_0236a48c;

typedef struct {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_0208922c;

extern UnkStruct_Boss *gBossF;
extern UnkStruct_0236a48c data_ov000_0236a48c;
extern UnkStruct_Player *gPlayers;
extern UnkStruct_0208922c gDisplayMain;

void BossFPartD8_SetAnimation(UnkStruct_022ef360 *s, s32 a, s32 b, s32 c);
void StageObj_ClearPause(void);
void BossEffect_CreateBlastF(s32 a, s32 x, s32 y, s32 z);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void BossFPartD8_State_DefeatFinish(void);

void BossFPartD8_State_DefeatExplode(UnkStruct_022ef360 *s) {
    UnkStruct_Boss *b = gBossF;
    u32 v;
    UnkStruct_0236a48c *c;

    BossFPartD8_SetAnimation(s, 0x25, 0x1000, 0);
    c = (UnkStruct_0236a48c *)(u32)&data_ov000_0236a48c;
    c->unk54 = 0x19e000;
    c->unk58 = 0xffd77000;
    c->unk5C = 0;
    c->unk6C = 0x1000;
    c->unk78 = 0x320000;
    c->unk80 = 0x1000;
    c->unk84 = -0x20000;
    c->unk8C = 0x1000;
    c->unk8E = 0xfaab;
    c->unk92 = 0x1000;
    v = (*(vu32 *)0x4000000 & 0x1f00) >> 8;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | ((v & ~4) << 8);
    b->base.unk54 &= ~0x20;
    s->base.unk54 &= ~0x20;
    b->unk358 = 0x1000;
    s->unk584 = 0x1000;
    StageObj_ClearPause();
    gPlayers->base.unk5C |= 0x20;
    gPlayers->unk32C |= 0x200000;
    gPlayers->unk424 = 0;
    gPlayers->unk422 = gPlayers->unk424;
    gPlayers->unk420 = gPlayers->unk422;
    BossEffect_CreateBlastF(0, s->base.unk10, s->base.unk14, s->base.unk18);
    NaviMsg_SetFaceAnim(0xd);
    NaviMsg_ShowMessage(0x13, 0xf0);
    gDisplayMain.unk48 = 0x10;
    Sound_PlaySeqArc(0, 0, 0x7d);
    s->unk434 = 0;
    s->base.unk68 = BossFPartD8_State_DefeatFinish;
}
