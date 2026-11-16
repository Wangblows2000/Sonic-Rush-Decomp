#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; } UnkStruct_020421b4;
void WirelessMgr_State_Lobby(void);
void WirelessMgr_State_LobbyOpen(UnkStruct_020421b4 *s) { s->unk8 = 1; s->unk1A = 0; s->fn = WirelessMgr_State_Lobby; }
