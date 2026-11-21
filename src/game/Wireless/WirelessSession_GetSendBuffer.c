#include <nitro/types.h>

#pragma thumb on

extern u32 gWirelessSendBuffer;
void *WirelessSession_GetSendBuffer(void) { return &gWirelessSendBuffer; }

#pragma thumb reset
