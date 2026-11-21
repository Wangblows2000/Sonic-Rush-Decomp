#include <nitro/types.h>

extern u16 data_0207cfe8;
void GX_VRAMCNT_SetLCDC_(u32 bank);
void GX_SetBankForLCDC(u32 bank) { data_0207cfe8 |= bank; GX_VRAMCNT_SetLCDC_(bank); }
