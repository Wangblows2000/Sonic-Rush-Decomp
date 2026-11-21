#include <nitro/types.h>

BOOL WMi_CheckInitialized(void);
s32 WM_GetLinkLevel(void);
s32 Wireless_GetLinkLevel(void) { if (WMi_CheckInitialized()) return 0; return WM_GetLinkLevel(); }
