#include <nitro/types.h>

u32 VramTex_Alloc(u32 a, BOOL b);
u32 OS_GetArenaLo(s32 a);
u32 VramTex_GfdAlloc(u32 a, BOOL b) { u32 r = VramTex_Alloc(a, b); if (r == OS_GetArenaLo(0)) return 0; return (r >> 3) | ((a >> 4) << 16) | (b ? 0x80000000 : 0); }
