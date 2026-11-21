#include <nitro/types.h>

typedef struct { u8 unk0[0x5e]; u16 unk5E; u8 unk60[0x68]; } UnkElem;
typedef struct { u8 mac[6]; u8 unk6[0xc2]; } UnkElemB;
typedef struct { UnkElem arr[32]; u8 pad[0x192a - 32 * 0xc8]; u16 cur; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
u16 WirelessMgr_GetPlayerCount(void);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void WirelessMgr_GetJoinInfo_0204319c(u16 *o) { UnkWork *w = gWirelessMgrTask->unk10; o[0] = w->arr[w->cur].unk5E; o[1] = WirelessMgr_GetPlayerCount(); MI_CpuCopy8(((UnkElemB *)((u8 *)w + 0x2c))[w->cur].mac, o + 2, 6); }
