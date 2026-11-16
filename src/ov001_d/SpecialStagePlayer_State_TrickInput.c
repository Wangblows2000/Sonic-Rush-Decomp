#include <nitro/types.h>

typedef struct UnkStruct_023115a4 {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[0x128 - 0x14];
    s32 unk128;
    u8 pad12C[0x14c - 0x12c];
    s16 unk14C;
    u16 unk14E;
    s16 unk150;
    s16 unk152;
    s32 unk154;
    s32 unk158;
} UnkStruct_023115a4;

extern s32 data_ov001_023178cc[];
extern u32 gRandSeed;

s16 SpecialStageTrick_CheckTouch(s16 a, u16 b, s16 c);
void SpecialStageCamera_SetMode(s32 v);
void SpecialStagePlayer_ChangeState(UnkStruct_023115a4 *p, s32 n);
void Sound_PlaySeqArc(void *p, u32 a, u32 b);
void SpecialStageMap_SetScrollSpeed(s32 v);
void SpecialStageHud_ShowTrickRating(s32 v);
s32 SpecialStageTrick_GetRating(s16 a, u16 b);
s16 func_ov001_02314694(s16 a);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void SpecialStagePlayer_State_TrickInput(UnkStruct_023115a4 *p) {
    s32 n;

    p->unk14C = SpecialStageTrick_CheckTouch(p->unk150, p->unk14E, p->unk14C);
    p->unk14E++;
    p->unk12 = 0;
    if (p->unk14C == -1) {
        SpecialStageCamera_SetMode(0);
        SpecialStagePlayer_ChangeState(p, 0xe);
        p->unk128 = 0;
        Sound_PlaySeqArc(0, 0x10, 0xb);
        Sound_PlaySeqArc(0, 0x10, 0x10);
        SpecialStageMap_SetScrollSpeed(0x800);
        SpecialStageHud_ShowTrickRating(3);
        return;
    }
    if (p->unk14C == 0x1234) {
        n = SpecialStageTrick_GetRating(p->unk150, p->unk14E);
        p->unk158 = n;
        p->unk154 = data_ov001_023178cc[n];
        p->unk152 = func_ov001_02314694(p->unk150);
        SpecialStageHud_ShowTrickRating(n);
        SpecialStagePlayer_ChangeState(p, Rand() % 3 + 0xb);
    }
}
