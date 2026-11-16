#include <nitro/types.h>

extern u16 data_0207fae8;
u32 WMi_CheckInitialized(void) { return data_0207fae8 != 0 ? 0 : 3; }
