#include <nitro/types.h>

#pragma thumb on

extern u32 data_022c463c;
u32 WirelessLobby_GetResult(void) { return data_022c463c; }

#pragma thumb reset
