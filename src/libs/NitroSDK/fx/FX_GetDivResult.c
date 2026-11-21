#include <nitro/types.h>

s32 FX_GetDivResult(void) { while (*(vu16 *)0x4000280 & 0x8000) {} return (s32)((*(volatile s64 *)0x40002a0 + (1 << 19)) >> 20); }
