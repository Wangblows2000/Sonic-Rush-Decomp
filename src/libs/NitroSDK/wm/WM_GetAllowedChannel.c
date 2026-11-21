#include <nitro/types.h>

u32 WMi_CheckInitialized(void);
u16 WM_GetAllowedChannel(void) { if (WMi_CheckInitialized() != 0) return 0x8000; return *(u16 *)0x27ffcfa; }
