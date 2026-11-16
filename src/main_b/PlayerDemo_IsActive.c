#include <nitro/types.h>

#pragma thumb on

extern void *gPlayerDemoTask;
BOOL PlayerDemo_IsActive(void) { if (gPlayerDemoTask != NULL) return TRUE; return FALSE; }

#pragma thumb reset
