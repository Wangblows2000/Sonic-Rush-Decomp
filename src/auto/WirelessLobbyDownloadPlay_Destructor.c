/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessMgr_StopDownloadPlay();
extern u8 gWirelessSession[];
extern u8 data_022c4638[];

void WirelessLobbyDownloadPlay_Destructor(s32 p0) {
    WirelessMgr_StopDownloadPlay();
    *(u16 *)gWirelessSession = 4;
    if ((s32)p0 == *(u32 *)data_022c4638) {
        *(u32 *)data_022c4638 = 0;
    }
}

#pragma thumb reset
