/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_GetStatus();
s32 WirelessMgr_GetPlayerCount();

s32 WirelessLobby_GetChildCount(void) {
    s32 t0 = WirelessMgr_GetStatus();
    if (t0 < 2) {
        return 0;
    }
    s32 t1 = WirelessMgr_GetPlayerCount(t0);
    return (t1 - 0x1);
}

#pragma thumb reset
