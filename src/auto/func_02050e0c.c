/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Wireless_EnableErrorCallback();
s32 func_02051838();
s32 func_0206cd7c();
s32 WirelessSession_StartDownloadPlay();

void func_02050e0c(s32 p0) {
    Wireless_EnableErrorCallback(0);
    func_02051838();
    func_0206cd7c(0);
    WirelessSession_StartDownloadPlay(p0);
}

#pragma thumb reset
