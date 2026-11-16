#include <nitro/types.h>

u32 VramTex_Alloc(u32 a, BOOL b);
u32 func_02006abc(s32 a);
u32 VramTex_GfdAlloc(u32 a, BOOL b) { u32 r = VramTex_Alloc(a, b); if (r == func_02006abc(0)) return 0; return (r >> 3) | ((a >> 4) << 16) | (b ? 0x80000000 : 0); }
