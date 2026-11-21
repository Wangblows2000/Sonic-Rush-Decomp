#include <nitro/types.h>

typedef struct UnkStruct_02356590 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_02356590;

u8 *WirelessSession_GetPartnerRecvBuffer(void);

#pragma thumb on
BOOL PlayerSelectMenu_HasPartnerCancelled(UnkStruct_02356590 *s) {
    if (s->unkC != 2)
        return FALSE;
    if (WirelessSession_GetPartnerRecvBuffer()[2] != 0)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
