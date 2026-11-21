#include <nitro/types.h>

#pragma thumb on

extern void *gWirelessErrorCallback;
void Wireless_OnError(void);
void Wireless_EnableErrorCallback(BOOL on) { if (on) gWirelessErrorCallback = (void *)Wireless_OnError; else gWirelessErrorCallback = NULL; }

#pragma thumb reset
