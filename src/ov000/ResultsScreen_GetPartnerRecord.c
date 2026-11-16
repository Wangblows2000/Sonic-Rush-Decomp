#include <nitro/types.h>

void WirelessSession_GetPartnerRecvBuffer(void);

#pragma thumb on
void ResultsScreen_GetPartnerRecord(void) {
    WirelessSession_GetPartnerRecvBuffer();
}
#pragma thumb reset
