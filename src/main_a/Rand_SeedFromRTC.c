#include <nitro/types.h>

typedef struct { u32 r; u32 g; u32 b; } UnkColor_0203492c;
extern vu32 gRandSeed;
BOOL RTC_GetTime(UnkColor_0203492c *c);

void Rand_SeedFromRTC(void) {
    UnkColor_0203492c c;
    u32 v;
    gRandSeed = 0;
    if (RTC_GetTime(&c)) return;
    v = c.b | (c.g << 6) | (c.r << 12);
    gRandSeed = v;
    gRandSeed = v | (v << 16);
}
