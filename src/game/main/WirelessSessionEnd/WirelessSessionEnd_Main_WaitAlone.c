#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; void (*fn)(void); } UnkTask;
extern UnkTask *gTaskCurrent;
BOOL Wireless_IsAlone(void);
void WirelessSession_Stop(void);
void WirelessSessionEnd_Main_WaitStopped(void);
void WirelessSessionEnd_Main_WaitAlone(void) { if (Wireless_IsAlone()) { WirelessSession_Stop(); gTaskCurrent->fn = WirelessSessionEnd_Main_WaitStopped; } }

#pragma thumb reset
