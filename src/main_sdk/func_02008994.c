#include <nitro/types.h>

void func_02008910(u32 cmd, u32 a, u32 b, u32 c, u32 d);
void func_02008994(u32 a, u32 b, u32 c, u32 d, u32 e) { func_02008910(7, a | (e << 24), b, c, d); }
