#include <nitro/types.h>

extern s16 data_0207cf7c;
s32 func_02068df8(s32 a, s32 b, s32 lim) { a += (b * data_0207cf7c << 8) >> 16; if (lim == 0) return a; if (b >= 0) { if (a > lim) a = lim; return a; } if (a < -lim) a = (s16)-lim; return a; }
