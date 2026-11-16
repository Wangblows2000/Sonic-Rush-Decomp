#include <nitro/types.h>

s32 Boss_Atan2(s32 a, s32 b);

void BossArena_CirclePosToArc(s32 unused, s32 len, s32 x, s32 y, s32 *out) {
    s32 t = (Boss_Atan2(x, y) << 12) / 0x10000;
    s32 r = (s32)(((s64)len * t + 0x800) >> 12);
    if (r < 0x40000)
        r += len;
    if (len + 0x40000 < r)
        r -= len;
    if (out != NULL)
        *out = r;
}
