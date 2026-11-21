#include <nitro/types.h>

typedef struct UnkStruct_0235fb24 {
    u8 pad[4];
    s32 unk4;
} UnkStruct_0235fb24;

u8 *WirelessSession_GetPartnerRecvBuffer(void);
u8 *WirelessSession_GetSendBuffer(void);

#pragma thumb on
BOOL TimeSelectMenu_IsPartnerReadyAsChooser(UnkStruct_0235fb24 *s) {
    u8 *p;
    if (s->unk4 != 0)
        p = WirelessSession_GetPartnerRecvBuffer();
    else
        p = WirelessSession_GetSendBuffer();
    if (p[3] == 0)
        return FALSE;
    return TRUE;
}
#pragma thumb reset
