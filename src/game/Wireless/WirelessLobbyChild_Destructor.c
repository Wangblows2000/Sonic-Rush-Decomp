#include <nitro/types.h>

extern u16 gWirelessSession;
extern void *gWirelessLobbyTask;
extern u32 gSystemFlags;
void WirelessMgr_StopDataSharing(s32 a);

#pragma thumb on
void WirelessLobbyChild_Destructor(void *task) {
    WirelessMgr_StopDataSharing(0);
    gWirelessSession = 4;
    if (task == gWirelessLobbyTask)
        gWirelessLobbyTask = NULL;
    gSystemFlags &= ~0x200;
}
#pragma thumb reset
