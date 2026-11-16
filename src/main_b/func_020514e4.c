#include <nitro/types.h>

#pragma thumb on

extern u32 gWirelessSendBuffer;
void *func_020514e4(void) { return &gWirelessSendBuffer; }

#pragma thumb reset
