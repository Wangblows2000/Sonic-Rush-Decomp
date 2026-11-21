#include <nitro/types.h>

extern u16 data_0207d368;
void OSi_SetTimerReserved(u32 n) { data_0207d368 |= 1 << n; }
