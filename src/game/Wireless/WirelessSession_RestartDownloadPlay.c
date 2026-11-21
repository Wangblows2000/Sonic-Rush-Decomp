/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Wireless_EnableErrorCallback();
s32 WirelessSession_ForceStop();
s32 NetPacket_Init();
s32 WirelessSession_StartDownloadPlay();

void WirelessSession_RestartDownloadPlay(s32 p0) {
    Wireless_EnableErrorCallback(0);
    WirelessSession_ForceStop();
    NetPacket_Init(0);
    WirelessSession_StartDownloadPlay(p0);
}

#pragma thumb reset
