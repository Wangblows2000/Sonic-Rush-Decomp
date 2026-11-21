#include <nitro/types.h>

u32 FX_GetSqrtResult(void) { while (*(vu16 *)0x40002b0 & 0x8000) {} return (*(vu32 *)0x40002b4 + 0x200) >> 10; }
