#include <nitro/types.h>

typedef struct UnkStruct_023113f8 {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[0x20 - 0x14];
    s32 x;
    s32 y;
    s32 z;
    u8 pad2C[0xac - 0x2c];
    u8 *unkAC;
    u8 padB0[0x128 - 0xb0];
    s32 unk128;
    u8 pad12C[0x150 - 0x12c];
    s16 unk150;
    s16 unk152;
    s32 unk154;
} UnkStruct_023113f8;

extern u32 gRandSeed;

void NNS_SndPlayerStopSeqBySeqArcIdx(s32 a, s32 b, s32 c);
void SpecialStageCamera_SetMode(s32 v);
void SpecialStagePlayer_ChangeState(UnkStruct_023113f8 *p, s32 v);
void SpecialStageMap_SetScrollSpeed(s32 v);
void SpecialStageHud_EndTrickRating(void);
void *SpecialStage_GetGimmickManager(void);
void SpecialStageGimmick_SpawnTrickReward(void *g, s32 x, s32 y, s32 z);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void SpecialStagePlayer_State_Trick(UnkStruct_023113f8 *p) {
    p->unk12 = 0;
    if (p->unkAC[0x10c] & 2) {
        NNS_SndPlayerStopSeqBySeqArcIdx(0x10, 0xf, 0x5a);
        if (p->unk152 >= 0) {
            p->unk150 = p->unk152;
            p->unk128 = 0;
            SpecialStagePlayer_ChangeState(p, 0xf);
            SpecialStageHud_EndTrickRating();
        } else {
            SpecialStageCamera_SetMode(0);
            SpecialStagePlayer_ChangeState(p, 0xe);
            p->unk128 = 0;
            SpecialStageMap_SetScrollSpeed(0x800);
        }
        return;
    }
    if (p->unk154 <= 0)
        return;
    SpecialStageGimmick_SpawnTrickReward(SpecialStage_GetGimmickManager(), p->x + (Rand() % 100 - 0x32) * 0x1000, p->y + (Rand() % 100 - 0x32) * 0x1000, p->z + (Rand() % 100 - 0x32) * 0x1000);
    p->unk154--;
}
