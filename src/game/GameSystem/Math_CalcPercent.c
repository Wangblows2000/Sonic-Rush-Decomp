/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Math_CalcPercent(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 v0 = ((u32)((((s32)p1 * 0x64) / (s32)p0) << 0x10) >> 0x10);
    if ((s32)p2 != 0) {
        v0 = ((u32)((v0 + ((u32)((s32)p3 * 0x64) / (u32)((s32)p0 * (s32)p2))) << 0x10) >> 0x10);
    }
    return v0;
}
