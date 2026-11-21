#include <nitro/types.h>

extern s32 gOptionsWfsHostTask;
void WirelessMgr_StopWfs(s32 a);

#pragma thumb on
void OptionsWfsHost_Destructor(void) {
    WirelessMgr_StopWfs(0);
    gOptionsWfsHostTask = 0;
}
#pragma thumb reset
