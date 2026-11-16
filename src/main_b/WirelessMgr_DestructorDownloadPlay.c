#include <nitro/types.h>

extern u32 gWirelessMgrTask;
void WirelessMgr_DestructorDownloadPlay(void) { gWirelessMgrTask = 0; }
