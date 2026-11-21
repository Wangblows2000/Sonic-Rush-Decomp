/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_StopDownloadPlay();
extern u8 gWirelessSession[];
extern u8 gWirelessLobbyTask[];

void WirelessLobbyDownloadPlay_Destructor(s32 p0) {
    WirelessMgr_StopDownloadPlay();
    *(u16 *)gWirelessSession = 4;
    if ((s32)p0 == *(u32 *)gWirelessLobbyTask) {
        *(u32 *)gWirelessLobbyTask = 0;
    }
}

#pragma thumb reset
