#include <nitro/types.h>

s64 FX_GetDivResultFx64c(void) { while (*(vu16 *)0x4000280 & 0x8000) {} return *(volatile s64 *)0x40002a0; }
