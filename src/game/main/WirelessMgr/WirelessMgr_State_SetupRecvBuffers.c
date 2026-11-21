#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; u8 unk1C[0x1ad2]; u16 unk1AEE; } UnkStruct_02041454;
void Wireless_SetupRecvBuffers(u16 v);
void WirelessMgr_State_WaitStart(void);
void WirelessMgr_State_SetupRecvBuffers(UnkStruct_02041454 *s) { Wireless_SetupRecvBuffers(s->unk1AEE); s->unk1A = 0; s->fn = WirelessMgr_State_WaitStart; }
