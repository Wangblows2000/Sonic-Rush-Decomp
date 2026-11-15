#include <nitro/types.h>

s32 func_02003f5c(u32 *out) { if (*(vu32 *)0x4000600 & 0x4000) return -1; *out = (*(vu32 *)0x4000600 & 0x1f00) >> 8; return 0; }
