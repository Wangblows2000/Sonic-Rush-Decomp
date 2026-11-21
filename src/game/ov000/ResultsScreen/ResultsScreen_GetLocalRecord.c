#include <nitro/types.h>

void WirelessSession_GetSendBuffer(void);

#pragma thumb on
void ResultsScreen_GetLocalRecord(void) {
    WirelessSession_GetSendBuffer();
}
#pragma thumb reset
