#include <nitro/types.h>

#pragma thumb on

extern void *gSysSound;
BOOL SysSound_IsInit(void) { if (gSysSound != NULL) return TRUE; return FALSE; }

#pragma thumb reset
