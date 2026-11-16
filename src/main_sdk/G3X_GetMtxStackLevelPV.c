#include <nitro/types.h>

s32 G3X_GetMtxStackLevelPV(u32 *out) { if (*(vu32 *)0x4000600 & 0x4000) return -1; *out = (*(vu32 *)0x4000600 & 0x1f00) >> 8; return 0; }
