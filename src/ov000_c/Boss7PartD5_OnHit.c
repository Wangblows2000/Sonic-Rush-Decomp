#include <ov000_c/stageobj.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8 pad12[0x18 - 0x12];
    void *unk18;
} UnkStruct_022eb5fc_Coll;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    s16 unk36;
    u8 pad38[0x58 - 0x38];
    u32 unk58;
    u8 pad5C[0x328 - 0x5C];
    s16 unk328;
    u8 pad32A[2];
    u32 unk32C;
    u8 pad330[0x3B2 - 0x330];
    s16 unk3B2;
    u8 pad3B4[0x420 - 0x3B4];
    s16 unk420;
    s16 unk422;
    s16 unk424;
} UnkStruct_022eb5fc_Pl;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x2E - 0x1C];
    s16 unk2E;
    u8 pad30[0x36 - 0x30];
    s16 unk36;
    u8 pad38[0x43C - 0x38];
    void *unk43C;
    void *unk440;
    u8 pad444[4];
    u32 unk448;
} UnkStruct_022eb5fc;

typedef struct {
    u8 pad0[0x220];
    u32 unk220;
} UnkStruct_02369b78;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_02369b78 *gBoss7;
extern UnkStruct_022c4560 gGameState;
extern UnkStruct_022eb5fc_Pl *gPlayers;

void Boss7PartD5_PushAway(UnkStruct_022eb5fc *w, UnkStruct_022eb5fc_Pl *pl);
void Boss7PartD5_CreateClashEffect(s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Player_OnHurt(UnkStruct_022eb5fc_Coll *a, UnkStruct_022eb5fc_Coll *b);
s32 RingManager_GetFlags(void);
void RingManager_SetFlags(s32 a);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);
void Boss7PartD5_InputState_Defeated(void);
void Boss7PartD5_MoveState_StartHurt(void);
void Player_Action_AttackBounce(UnkStruct_022eb5fc_Pl *pl);
void Boss7PartD5_CreateHitEffect(s32 x, s32 y, s32 z);

static inline void Hit(UnkStruct_02369b78 *mgr, UnkStruct_022eb5fc *w, UnkStruct_022eb5fc_Pl *pl) {
    u32 v = mgr->unk220;
    if (v - w->unk448 < 4)
        return;
    w->unk448 = v;
    Boss7PartD5_PushAway(w, pl);
    Boss7PartD5_CreateClashEffect((w->unk10 + pl->unk10) >> 1, (w->unk14 + pl->unk14) >> 1, (w->unk18 + pl->unk18) >> 1);
    Sound_PlaySeqArc(0, 0, 0xcb);
}

void Boss7PartD5_OnHit(UnkStruct_022eb5fc_Coll *a, UnkStruct_022eb5fc_Coll *b) {
    UnkStruct_02369b78 *mgr = gBoss7;
    UnkStruct_022eb5fc *w = a->unk18;
    UnkStruct_022eb5fc_Pl *pl = b->unk18;
    u16 hp = gBossStatus.unk8;
    s32 spd;

    if (hp == 1)
        return;
    if (gBossStatus.unk4 == 0x29) {
        if (pl->unk3B2 > 0)
            return;
        if ((u16)(s16)(pl->unk328 - 0x30) <= 1) {
            Hit(mgr, w, pl);
            return;
        }
        if (pl->unk3B2 > 0)
            return;
        Player_OnHurt(b, a);
        return;
    }
    if (gBossStatus.unk4 == 0x28) {
        if (pl->unk328 == 0x30) {
            Hit(mgr, w, pl);
            return;
        }
        if (pl->unk3B2 > 0)
            return;
        Player_OnHurt(b, a);
        return;
    }
    if ((u16)(s16)(pl->unk328 - 0x30) <= 1) {
        gBossStatus.unk8 = hp - 1;
        if (gBossStatus.unk8 <= 1) {
            gBossStatus.unk8 = 1;
            gGameState.unk18 &= ~1;
            gPlayers->unk32C |= 0x200000;
            gPlayers->unk420 = 0;
            gPlayers->unk422 = 0;
            gPlayers->unk424 = 0;
            RingManager_SetFlags(RingManager_GetFlags() | 1);
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            w->unk43C = Boss7PartD5_InputState_Defeated;
        } else {
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x10, 0xf0);
        }
        w->unk440 = Boss7PartD5_MoveState_StartHurt;
        Player_Action_AttackBounce(gPlayers);
        Boss7PartD5_CreateHitEffect(b->unk0 << 8, b->unk4 << 8, 0);
        return;
    }
    if (pl->unk3B2 > 0)
        return;
    if (w->unk10 < pl->unk10) {
        pl->unk36 = (((a->unk0 + a->unk10) - b->unkC) << 8) - pl->unk10;
        spd = 0x400;
    } else {
        pl->unk36 = (((a->unk0 + a->unkC) - b->unk10) << 8) - pl->unk10;
        spd = -0x400;
    }
    pl->unk36 >>= 1;
    w->unk36 = -pl->unk36;
    if (pl->unk58 & 0x8000)
        pl->unk2E = spd;
    else
        pl->unk34 = spd;
    w->unk2E = -spd;
}
