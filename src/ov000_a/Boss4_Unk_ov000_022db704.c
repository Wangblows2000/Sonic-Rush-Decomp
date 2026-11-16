#include <nitro/types.h>

extern u32 gRandSeed;

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

BOOL Boss4_Unk_ov000_022db704(u32 mask, s32 n) {
    if (mask & (1 << (Rand() % n)))
        return TRUE;
    return FALSE;
}
