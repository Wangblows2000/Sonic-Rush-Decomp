#include <nitro/types.h>

void func_02008910(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void func_02008c60(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) { func_02008910(0x11, c, d, (a << 31) | (b << 30) | (e << 29) | (f << 28) | (g << 27), 0); }
