#include <nitro/types.h>

s32 func_0200a0d8(u32 tag, u32 data, u32 err);
BOOL func_0200f1e8(u32 cmd) { return func_0200a0d8(5, (cmd << 8) & 0x7f00, 0) >= 0; }
