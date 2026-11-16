/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessSession_ForceStop();
s32 NetPacket_Init();
s32 WirelessLobby_CreateParent();

void WirelessLobby_StartParent(s32 p0) {
    WirelessSession_ForceStop();
    NetPacket_Init(0);
    WirelessLobby_CreateParent(p0);
}

#pragma thumb reset
