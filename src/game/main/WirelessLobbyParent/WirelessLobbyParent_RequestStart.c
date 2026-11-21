#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; u32 unk8; s32 unkC; u32 unk10; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
void WirelessLobbyParent_RequestStart(void) { s32 v; if (gWirelessLobbyTask == NULL) v = 7; else v = gWirelessLobbyTask->unk10->unkC; if (v >= 3) gWirelessLobbyTask->unk10->unk10 = 1; }

#pragma thumb reset
