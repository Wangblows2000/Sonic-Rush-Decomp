#include <nitro/types.h>

extern u32 data_0207d3a4;
extern u16 data_0207d3a8[];
void OSi_InitVramExclusive(void) { s32 i; data_0207d3a4 = 0; for (i = 0; i < 9; i++) data_0207d3a8[i] = 0; }
