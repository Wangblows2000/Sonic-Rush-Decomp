#include <nitro/types.h>

u32 func_0203969c(u32 a, BOOL b);
u32 func_02006abc(s32 a);
u32 func_0203938c(u32 a, BOOL b) { u32 r = func_0203969c(a, b); if (r == func_02006abc(0)) return 0; return (r >> 3) | ((a >> 4) << 16) | (b ? 0x80000000 : 0); }
