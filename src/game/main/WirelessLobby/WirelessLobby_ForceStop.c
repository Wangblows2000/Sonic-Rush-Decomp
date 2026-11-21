#include <nitro/types.h>

#pragma thumb on

void WirelessSession_ForceStop(void);
void WirelessLobby_ForceStop(void) { WirelessSession_ForceStop(); }

#pragma thumb reset
