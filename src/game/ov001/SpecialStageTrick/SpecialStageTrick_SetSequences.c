#include <nitro/types.h>

extern u8 *gSpecialStageTrick;

void SpecialStageTrick_SetSequences(s32 a, s32 b) {
    u8 *p = gSpecialStageTrick;
    *(s32 *)(p + 0x10c) = a;
    *(s32 *)(p + 0x110) = b;
}
