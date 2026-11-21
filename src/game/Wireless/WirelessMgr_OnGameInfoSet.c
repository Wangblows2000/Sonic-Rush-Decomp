#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
void WirelessMgr_OnGameInfoSet(void) { gWirelessMgrTask->unk10[9] = 0; }
