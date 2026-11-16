#include <nitro/types.h>

#pragma thumb on

extern void *data_022c4638;
extern void *data_022c4634;
BOOL WirelessSession_IsStopped(void) { if (data_022c4638 != NULL) return FALSE; if (data_022c4634 == NULL) return TRUE; return FALSE; }

#pragma thumb reset
