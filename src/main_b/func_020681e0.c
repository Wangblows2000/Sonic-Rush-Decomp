#include <nitro/types.h>

s32 func_020681e0(s32 a, s32 b) { a = (a >> 8) - (b >> 8); if (a < -128) return -128; if (a > 127) a = 127; return a; }
