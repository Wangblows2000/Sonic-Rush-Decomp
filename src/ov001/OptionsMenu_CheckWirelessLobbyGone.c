#include <nitro/types.h>

void WirelessLobby_IsGone(void);

#pragma thumb on
void OptionsMenu_CheckWirelessLobbyGone(void) {
    WirelessLobby_IsGone();
}
#pragma thumb reset
