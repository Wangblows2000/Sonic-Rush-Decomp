#include <nitro/types.h>

typedef struct { u32 r; u32 g; u32 b; } UnkColor_0203492c;
extern vu32 gRandSeed;
BOOL func_0200f0a0(UnkColor_0203492c *c);

void Rand_SeedFromRTC(void) {
    UnkColor_0203492c c;
    u32 v;
    gRandSeed = 0;
    if (func_0200f0a0(&c)) return;
    v = c.b | (c.g << 6) | (c.r << 12);
    gRandSeed = v;
    gRandSeed = v | (v << 16);
}
