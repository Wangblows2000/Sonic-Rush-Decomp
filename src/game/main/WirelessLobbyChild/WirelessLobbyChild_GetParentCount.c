#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
u16 WirelessLobbyChild_GetParentCount(void) { return gWirelessLobbyTask->unk10[0x1a]; }

#pragma thumb reset
