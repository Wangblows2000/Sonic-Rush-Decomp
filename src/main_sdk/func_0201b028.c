#include <nitro/types.h>

void func_0201b060(u32 a, u32 *t, u32 b, u32 c);
u32 func_0201b028(u32 a, u32 b, u32 c) { u32 t = 0xffffffff; func_0201b060(a, &t, b, c); return ~t; }
