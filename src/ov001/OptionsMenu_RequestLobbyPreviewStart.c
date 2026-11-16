#include <nitro/types.h>

void WirelessLobbyPreview_RequestStart(void);

#pragma thumb on
void OptionsMenu_RequestLobbyPreviewStart(void) {
    WirelessLobbyPreview_RequestStart();
}
#pragma thumb reset
