#include <nitro/types.h>

s32 func_0200a0d8(u32 tag, u32 data, u32 err);
void func_0200774c(u32 id) { while (func_0200a0d8(0xc, id << 8, 0) != 0) {} }
