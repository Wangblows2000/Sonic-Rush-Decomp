#include <nitro/types.h>

s32 Math_LerpFx32(s32 a, s32 b, s32 t) { if (t == 0) return b; if (t != 0x1000) a = b + (((a - b) * t) >> 12); return a; }
