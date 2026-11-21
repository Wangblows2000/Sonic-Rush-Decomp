#include <nitro/types.h>

typedef struct UnkStruct_0235fb50 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_0235fb50;

u8 *WirelessSession_GetSendBuffer(void);
u8 *WirelessSession_GetPartnerRecvBuffer(void);

#pragma thumb on
BOOL TimeSelectMenu_IsPartnerReadyAsFollower(UnkStruct_0235fb50 *s) {
    u8 *p;
    if (s->unk4 != 0)
        p = WirelessSession_GetSendBuffer();
    else
        p = WirelessSession_GetPartnerRecvBuffer();
    if (p[3] == 0)
        return FALSE;
    return TRUE;
}
#pragma thumb reset
