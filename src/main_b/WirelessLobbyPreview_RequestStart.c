#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
void WirelessLobbyPreview_RequestStart(void) { gWirelessLobbyTask->unk10[2] = 1; }

#pragma thumb reset
