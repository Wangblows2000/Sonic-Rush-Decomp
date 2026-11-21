#include <nitro/types.h>

typedef struct { u8 unk0[0x192a]; u16 unk192A; } UnkWork;
typedef struct { u8 unk0[0x10]; UnkWork *unk10; } UnkTask;
extern UnkTask *gWirelessMgrTask;
u32 WirelessMgr_GetStatus(void);
u32 WH_GetSystemState(void);
BOOL WH_EndScan(void);
void WirelessMgr_SelectParent(u16 v) { if (WirelessMgr_GetStatus() != 4) return; if (WH_GetSystemState() == 2) WH_EndScan(); gWirelessMgrTask->unk10->unk192A = v; }
