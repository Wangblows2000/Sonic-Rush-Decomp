#include <nitro/types.h>

extern u16 data_0207cfe8;
void GX_VRAMCNT_SetLCDC_(u32 bank);
u32 resetBankForX_(u16 *p) { u32 v = *p; *p = 0; data_0207cfe8 |= v; GX_VRAMCNT_SetLCDC_(v); return v; }
