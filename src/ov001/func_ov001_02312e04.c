#include <nitro/types.h>

extern u32 gRandSeed;
extern s32 data_ov001_023178e4[];
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov001_023122d8(void *p, s32 v);
static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void func_ov001_02312e04(void *p) {
    Sound_PlaySeqArc(0, 0x10, data_ov001_023178e4[(s32)Rand() % 4]);
    func_ov001_023122d8(p, 0x14);
}
