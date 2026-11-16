#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
} UnkStruct_02356538;

UnkStruct_02356538 *WirelessSession_GetSendBuffer(void);
UnkStruct_02356538 *WirelessSession_GetPartnerRecvBuffer(void);

#pragma thumb on

BOOL PlayerSelectMenu_IsDifferentChoiceFromPartner(void) {
    UnkStruct_02356538 *a = WirelessSession_GetSendBuffer();
    UnkStruct_02356538 *b = WirelessSession_GetPartnerRecvBuffer();

    if (a->unk1 == 0 || b->unk1 == 0) {
        return FALSE;
    }
    if (a->unk0 != b->unk0) {
        return TRUE;
    }
    return FALSE;
}

#pragma thumb reset
