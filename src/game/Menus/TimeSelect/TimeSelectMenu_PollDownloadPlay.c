#include <nitro/types.h>

void WirelessSession_IsDownloadPlay(void);

#pragma thumb on
void TimeSelectMenu_PollDownloadPlay(void) {
    WirelessSession_IsDownloadPlay();
}
#pragma thumb reset
