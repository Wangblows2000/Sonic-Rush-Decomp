#include <nitro/types.h>

u32 VramPltt_Alloc(u32 v);
u32 OS_GetArenaLo(s32 a);
u32 VramPltt_GfdAlloc(u32 v) { u32 r = VramPltt_Alloc((u16)(v >> 1)); if (r == OS_GetArenaLo(0)) return 0; return (r >> 3) | ((v >> 3) << 16); }
