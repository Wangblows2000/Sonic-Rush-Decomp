#include <nitro/types.h>

void PMi_SetLCDPower(u32 a, u32 b, u32 c, u32 d);
void PM_SetLCDPower(u32 a) { PMi_SetLCDPower(a == 1 ? a : 0, 0, 0, 1); }
