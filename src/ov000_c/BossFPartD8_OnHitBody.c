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
} UnkStruct_022f50b0_Coll;

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
    u8 pad5C[0x328 - 0x5C];
    s16 unk328;
} UnkStruct_022f50b0_Pl;

typedef struct {
    u32 unk0;
    s32 unk4;
    u16 unk8;
    u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x20C];
    u32 unk20C;
} UnkStruct_02369b84;

extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_022f50b0_Pl *gPlayers;
extern UnkStruct_02369b84 *gBossF;

void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void BossFPartD8_CreateHitEffect(s32 x, s32 y, s32 z);
void BossFPartD8_TriggerHurtInSky(void);
void BossFPartD8_TriggerDefeat(void);
void BossFPartD8_TriggerHurt(void);
void NaviMsg_SetFaceAnim(s32 a);
void NaviMsg_ShowMessage(s32 a, s32 b);

void BossFPartD8_OnHitBody(UnkStruct_022f50b0_Coll *a, UnkStruct_022f50b0_Coll *b) {
    UnkStruct_022f50b0_Pl *pl = b->unk18;
    UnkStruct_StageObj *w = a->unk18;
    u16 hp = gBossStatus.unk8;
    s16 e1;
    s16 e2;
    s32 v;

    if (hp == 0)
        return;
    if (gBossStatus.unk4 == 0x38) {
        gBossStatus.unk8 = hp - 1;
        if (gBossStatus.unk8 != 0) {
            Sound_PlaySeqArc(0, 0, 0x7b);
            Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
            BossFPartD8_CreateHitEffect(b->unk0 << 8, b->unk4 << 8, 0);
            BossFPartD8_TriggerHurtInSky();
            return;
        }
        BossFPartD8_TriggerDefeat();
        return;
    }
    if (gPlayers->unk328 == 0x30) {
        gBossStatus.unk8 = hp - 1;
        Sound_PlaySeqArc(0, 0, 0x7b);
        Sound_PlayVoice(0, 0, (Rand() & 1) ? 0xce : 0xcf);
        BossFPartD8_CreateHitEffect(b->unk0 << 8, b->unk4 << 8, 0);
        BossFPartD8_TriggerHurt();
        pl->unk36 = (((a->unk0 + ((w->unk5C & 1) ? a->unkC : (s16)-a->unk10)) - b->unk10) << 8) - pl->unk10;
        pl->unk58 |= 4;
        if (gBossStatus.unk8 <= 1) {
            if (gBossF->unk20C & 1)
                return;
            NaviMsg_SetFaceAnim(0xb);
            NaviMsg_ShowMessage(0x12, 0xf0);
            return;
        }
        NaviMsg_SetFaceAnim(0xb);
        NaviMsg_ShowMessage(0x10, 0xf0);
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
    if ((a->unk0 + ((e1 + e2) >> 1)) << 8 < pl->unk10) {
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
