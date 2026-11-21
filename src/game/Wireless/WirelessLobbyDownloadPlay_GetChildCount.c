#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x10]; u16 *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
u16 WirelessLobbyDownloadPlay_GetChildCount(void) { return gWirelessLobbyTask->unk10[0x1b]; }

#pragma thumb reset
