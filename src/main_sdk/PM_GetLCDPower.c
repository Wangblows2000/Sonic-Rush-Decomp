#include <nitro/types.h>

BOOL PM_GetLCDPower(void) { return (*(vu16 *)0x4000304 & 1) ? 1 : 0; }
