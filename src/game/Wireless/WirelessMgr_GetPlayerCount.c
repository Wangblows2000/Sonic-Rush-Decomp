#include <nitro/types.h>

typedef struct { u8 unk0[0x14]; u16 unk14; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
void Math_CountBits16(u16 v);
void WirelessMgr_GetPlayerCount(void) { Math_CountBits16(gWirelessMgrTask->unk10->unk14); }
