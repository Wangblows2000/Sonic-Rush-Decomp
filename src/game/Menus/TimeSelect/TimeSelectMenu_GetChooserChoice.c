#include <nitro/types.h>

typedef struct UnkStruct_0235fbcc {
    u8 pad[4];
    s32 unk4;
} UnkStruct_0235fbcc;

u8 *WirelessSession_GetSendBuffer(void);
u8 *WirelessSession_GetPartnerRecvBuffer(void);

#pragma thumb on
u8 TimeSelectMenu_GetChooserChoice(UnkStruct_0235fbcc *s) {
    u8 *p;
    if (s->unk4 != 0)
        p = WirelessSession_GetSendBuffer();
    else
        p = WirelessSession_GetPartnerRecvBuffer();
    return *p;
}
#pragma thumb reset
