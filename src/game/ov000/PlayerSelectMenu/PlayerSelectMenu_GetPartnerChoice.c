#include <nitro/types.h>

u8 *WirelessSession_GetPartnerRecvBuffer(void);

#pragma thumb on
u8 PlayerSelectMenu_GetPartnerChoice(void) {
    u8 *p = WirelessSession_GetPartnerRecvBuffer();
    if (p[1] == 0)
        return 4;
    return p[0];
}
#pragma thumb reset
