#include <game/Stage/stageobj.h>

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
    u8 pad[0x114];
    s32 unk114;
} UnkStruct_Mdl;

typedef struct {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022c4560;

typedef struct UnkStruct_022ed428 UnkStruct_022ed428;
struct UnkStruct_022ed428 {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_StageObj *unk214;
    u8 pad218[0x232 - 0x218];
    u16 unk232;
    u8 pad234[0x388 - 0x234];
    s32 unk388;
    u8 pad38C[0x394 - 0x38C];
    u16 unk394;
};

extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_StageObj *gPlayers;
extern UnkStruct_022c4560 gGameState;
extern u8 gBGMSoundHandle[];

void BossEffect_CreateStrikeBig(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Player_Action_Blank(UnkStruct_StageObj *p);
void Player_SetAnimation(UnkStruct_StageObj *p, s32 a);
void Boss7_SetRivalAnimation(UnkStruct_StageObj *p, s32 a);
void Boss7_SetCollidersEnabled(UnkStruct_StageObj *p, s32 a, s32 b);
void ScreenShake_Start(s32 a);
void BossHelp_Create(s32 a);
void Sound_PlaySeq(void *a, s32 b);
void Boss7_State_ClashStruggle(void);

void Boss7_State_ClashStart(UnkStruct_022ed428 *s) {
    UnkStruct_StageObj *o = s->unk214;
    UnkStruct_StageObj *p = gPlayers;

    BossEffect_CreateStrikeBig(1, (o->unk10 + p->unk10) >> 1, (o->unk14 + p->unk14) >> 1,
                        ((o->unk18 + p->unk18) >> 1) + 0x1400);
    Sound_PlaySeqArc(s->unk388, 0, 0xbc);
    Sound_PlaySeqArc(0, 0, 0x9a);
    s->unk232 = 0xe00;
    gDisplayMain.unk20.raw = 0;
    gDisplayMain.unk20.bits.b0 = 1;
    gDisplayMain.unk20.bits.b6 = 2;
    gDisplayMain.unk24.b0 = (u16)(s->unk232 >> 8);
    Player_Action_Blank(p);
    Player_SetAnimation(p, 0x42);
    p->unk5C |= 4;
    ((UnkStruct_Mdl *)p->unk124)->unk114 = 0x4000;
    Boss7_SetRivalAnimation(o, 0x42);
    Boss7_SetCollidersEnabled(o, 0, 0);
    o->unk5C |= 4;
    ((UnkStruct_Mdl *)o->unk124)->unk114 = 0xc000;
    s->unk394 = 0;
    ScreenShake_Start(8);
    BossHelp_Create(0);
    Sound_PlaySeq(gBGMSoundHandle, 0x11);
    gGameState.unk18 |= 1;
    s->base.unk68 = Boss7_State_ClashStruggle;
}
