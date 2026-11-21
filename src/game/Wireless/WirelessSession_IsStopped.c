#include <nitro/types.h>

#pragma thumb on

extern void *gWirelessLobbyTask;
extern void *data_022c4634;
BOOL WirelessSession_IsStopped(void) { if (gWirelessLobbyTask != NULL) return FALSE; if (data_022c4634 == NULL) return TRUE; return FALSE; }

#pragma thumb reset
