#include <nitro/types.h>

extern u16 gWirelessSession;
extern void *data_022c4638;
extern u32 gSystemFlags;
void WirelessMgr_StopDataSharing(s32 a);

#pragma thumb on
void WirelessLobbyParent_Destructor(void *task) {
    WirelessMgr_StopDataSharing(0);
    gWirelessSession = 4;
    if (task == data_022c4638)
        data_022c4638 = NULL;
    gSystemFlags &= ~0x200;
}
#pragma thumb reset
