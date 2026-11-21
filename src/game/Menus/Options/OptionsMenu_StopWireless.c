#include <nitro/types.h>

void WirelessLobby_ForceStop(void);
void OptionsWfsHost_Destroy(void);

#pragma thumb on
void OptionsMenu_StopWireless(void) {
    WirelessLobby_ForceStop();
    OptionsWfsHost_Destroy();
}
#pragma thumb reset
