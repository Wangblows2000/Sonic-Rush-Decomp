/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 WirelessSession_ForceStop();
s32 NetPacket_Init();
s32 WirelessLobby_CreateChild();

void WirelessLobby_StartChild(s32 p0) {
    WirelessSession_ForceStop();
    NetPacket_Init(0);
    WirelessLobby_CreateChild(p0);
}

#pragma thumb reset
