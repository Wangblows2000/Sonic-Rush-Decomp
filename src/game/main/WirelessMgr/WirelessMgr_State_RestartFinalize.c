#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x16]; u16 unk1A; } UnkStruct_0204189c;
extern u32 gSystemFlags;
extern u8 gWirelessSendBuffer;
void WH_Finalize(void);
void MIi_CpuClear32(u32 data, void *dst, u32 size);
void WirelessMgr_State_ChildReconnect(void);
void WirelessMgr_State_RestartFinalize(UnkStruct_0204189c *s) { vu32 z; gSystemFlags &= ~0x100; gSystemFlags |= 0x200; WH_Finalize(); z = 0; MIi_CpuClear32(z, &gWirelessSendBuffer, 4); s->unk1A = 0; s->fn = WirelessMgr_State_ChildReconnect; }
