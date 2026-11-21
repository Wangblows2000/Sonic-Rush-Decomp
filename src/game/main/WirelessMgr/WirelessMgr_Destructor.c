#include <nitro/types.h>

extern u32 gWirelessMgrTask;
void WirelessMgr_Destructor(void) { gWirelessMgrTask = 0; }
