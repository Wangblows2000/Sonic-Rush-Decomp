#include <nitro/types.h>

extern u32 gRandSeed;
extern s32 data_ov001_023178e4[];
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void SpecialStagePlayer_ChangeState(void *p, s32 v);
static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void SpecialStagePlayer_Action_Celebrate(void *p) {
    Sound_PlaySeqArc(0, 0x10, data_ov001_023178e4[(s32)Rand() % 4]);
    SpecialStagePlayer_ChangeState(p, 0x14);
}
