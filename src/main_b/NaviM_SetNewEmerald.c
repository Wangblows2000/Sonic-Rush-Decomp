#include <nitro/types.h>

extern u16 gNaviMNewEmeraldMask;
void NaviM_SetNewEmerald(s32 n) { gNaviMNewEmeraldMask = 1 << n; }
