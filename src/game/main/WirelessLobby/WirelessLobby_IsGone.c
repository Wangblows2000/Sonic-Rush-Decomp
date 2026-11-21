#include <nitro/types.h>

#pragma thumb on

extern void *gWirelessLobbyTask;
BOOL WirelessLobby_IsGone(void) { if (gWirelessLobbyTask == NULL) return TRUE; return FALSE; }

#pragma thumb reset
