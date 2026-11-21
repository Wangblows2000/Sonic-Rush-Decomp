#include <nitro/types.h>

typedef struct { void (*fn)(void); u32 unk4; u32 unk8; u8 unkC[0xe]; u16 unk1A; } UnkStruct_020419e4;
void WirelessMgr_State_LobbyWaitStart(void);
void WirelessMgr_State_ChildJoined(UnkStruct_020419e4 *s) { s->unk8 = 5; s->unk1A = 0; s->fn = WirelessMgr_State_LobbyWaitStart; }
