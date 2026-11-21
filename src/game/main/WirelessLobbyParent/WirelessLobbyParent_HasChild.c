#include <nitro/types.h>

#pragma thumb on

typedef struct { u32 unk0; u32 unk4; u32 unk8; s32 unkC; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
BOOL WirelessLobbyParent_HasChild(void) { s32 v; if (gWirelessLobbyTask == NULL) v = 7; else v = gWirelessLobbyTask->unk10->unkC; if (v < 3) return FALSE; return TRUE; }

#pragma thumb reset
