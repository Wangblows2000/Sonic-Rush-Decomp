#include <nitro/types.h>

BOOL OSi_IsRunOnDebugger(void) { u16 *p; if (*(vu16 *)0x27ffc10 == 0) p = (u16 *)0x27ff814; else p = (u16 *)0x27ffc14; return *p == 1; }
