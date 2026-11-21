#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
void WirelessLobbyDownloadPlay_RequestRestart(void) { gWirelessLobbyTask->unk10[6] = 1; }

#pragma thumb reset
