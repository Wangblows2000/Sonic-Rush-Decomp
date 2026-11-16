#include <nitro/types.h>

u32 MBi_calc_nextsendblock(u32 a, u32 b) { if (b <= a) b = a; return b; }
