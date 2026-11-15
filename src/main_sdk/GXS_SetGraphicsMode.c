#include <nitro/types.h>

void GXS_SetGraphicsMode(u32 mode) { *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~7) | mode; }
