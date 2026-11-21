#include <nitro/types.h>

void GX_SetVCountEqVal(s32 v) { *(vu16 *)0x4000004 = (u16)((*(vu16 *)0x4000004 & 0x3f) | ((v & 0xff) << 8) | ((v & 0x100) >> 1)); }
