#include <nitro/types.h>

extern u16 data_0208af8a[];
u32 OS_GetArenaLo(s32 a);
void VramPltt_Free(u32 addr) { u32 i; if (addr == OS_GetArenaLo(0)) return; if (addr < 0x10000) i = addr >> 3; else i = ((addr - 0x10000) >> 5) + 0x2000; data_0208af8a[i] = 0; }
