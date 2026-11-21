/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_GetStatus();
s32 Wireless_OnError();
extern u8 gWirelessSession[];
s32 Wireless_EnableErrorCallback();
s32 WirelessSession_CachePartnerName();

s32 WirelessSession_PollConnected(void) {
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 == 7) {
        Wireless_OnError();
        return 0;
    }
    s32 t1 = WirelessMgr_GetStatus(t0);
    if (t1 != 6) {
        return 0;
    }
    if (*(u16 *)gWirelessSession == 0) {
        Wireless_EnableErrorCallback(1);
        WirelessSession_CachePartnerName();
    } else {
        Wireless_EnableErrorCallback(0);
    }
    return 1;
}

#pragma thumb reset
