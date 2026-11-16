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
} UnkStruct_022f4ec8_Coll;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    s16 unk36;
    u8 pad38[0x58 - 0x38];
    u32 unk58;
    u8 pad5C[0x17C - 0x5C];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
    u8 pad188[0x324 - 0x188];
    u8 unk324;
    u8 pad325[0x328 - 0x325];
    s16 unk328;
} UnkStruct_022f4ec8_Pl;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_022f4ec8_Pl *gPlayers;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Player_Action_AttackBounce(UnkStruct_022f4ec8_Pl *p);
void BossEffect_CreateDmgHand(s32 a, s32 x, s32 y, s32 z);
void func_ov000_022f8948(void);
void func_ov000_022f8904(void);
void func_ov000_022f88c0(void);

void BossFPartD8_OnHit_Unk_ov000_022f4ec8(UnkStruct_022f4ec8_Coll *a, UnkStruct_022f4ec8_Coll *b) {
    UnkStruct_022f4ec8_Pl *pl = b->unk18;
    UnkStruct_StageObj *w = a->unk18;
    s16 e1;
    s16 e2;
    s32 v;

    if (pl->unk328 == 0x30) {
        Player_Action_AttackBounce(pl);
        if (gBossStatus.unk4 == 0x32) {
            if (gPlayers->unk324 == 0)
                pl->unk2E = -0x800;
            else
                pl->unk2E = -0xb00;
        }
        BossEffect_CreateDmgHand(1, pl->unk17C >> 4, -pl->unk180 >> 4, pl->unk184 >> 4);
        Sound_PlaySeqArc(0, 0, 0x7b);
        switch (gBossStatus.unk4) {
        case 0x30:
            func_ov000_022f8948();
            break;
        case 0x31:
            func_ov000_022f8904();
            break;
        case 0x32:
            func_ov000_022f88c0();
            break;
        }
        return;
    }
    if (w->unk5C & 1) {
        e1 = a->unkC;
        e2 = a->unk10;
    } else {
        e1 = -a->unk10;
        e2 = -a->unkC;
    }
    v = (pl->unk58 & 0x8000) ? pl->unk2E : pl->unk34;
    if (a->unk0 << 8 < pl->unk10) {
        if (v > 0)
            return;
        pl->unk36 = (((a->unk0 + e2) - b->unkC) << 8) - pl->unk10;
        pl->unk58 |= 4;
        pl->unk2E = 0;
        pl->unk34 = 0;
    } else {
        if (v < 0)
            return;
        pl->unk36 = (((a->unk0 + e1) - b->unk10) << 8) - pl->unk10;
        pl->unk58 |= 4;
        pl->unk2E = 0;
        pl->unk34 = 0;
    }
}
