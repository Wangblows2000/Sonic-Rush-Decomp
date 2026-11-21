/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gWirelessLobbyTask[];
s32 Wireless_GetLinkLevel();

s32 WirelessLobby_GetLinkLevel(void) {
    s32 t0 = *(u32 *)gWirelessLobbyTask;
    if (t0 == 0) {
        return 0;
    }
    s32 t1 = Wireless_GetLinkLevel(t0);
    return t1;
}

#pragma thumb reset
