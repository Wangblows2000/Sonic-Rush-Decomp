#include <nitro/types.h>

u32 VramPltt_Alloc(u32 v);
u32 func_02006abc(s32 a);
u32 VramPltt_GfdAlloc(u32 v) { u32 r = VramPltt_Alloc((u16)(v >> 1)); if (r == func_02006abc(0)) return 0; return (r >> 3) | ((v >> 3) << 16); }
