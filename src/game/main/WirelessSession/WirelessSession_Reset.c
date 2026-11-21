#include <nitro/types.h>

#pragma thumb on

extern void *gWirelessLobbyTask;
extern void *data_022c4634;
extern u16 gWirelessSession[2];
void WirelessSession_Reset(void) { gWirelessLobbyTask = NULL; data_022c4634 = NULL; gWirelessSession[0] = 4; gWirelessSession[1] = 5; }

#pragma thumb reset
