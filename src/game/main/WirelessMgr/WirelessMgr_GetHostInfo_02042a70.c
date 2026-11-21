#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u16 unk10; u16 unk12; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
u16 WirelessMgr_GetPlayerCount(void);
void WirelessMgr_GetHostInfo_02042a70(u16 *out) { UnkWork *w = gWirelessMgrTask->unk10; out[0] = w->unk10; out[1] = WirelessMgr_GetPlayerCount(); out[2] = (w->unk12 + 1) | 0x8000; }
