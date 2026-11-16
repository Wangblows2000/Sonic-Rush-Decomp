#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x34]; u16 unk34; u8 unk36[0xd2]; u16 unk108[1]; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessLobbyTask;
u16 WirelessLobbyChild_Unk_02050ec0(s32 i) { UnkWork *w = gWirelessLobbyTask->unk10; if (i >= w->unk34) return 0; return w->unk108[i]; }

#pragma thumb reset
