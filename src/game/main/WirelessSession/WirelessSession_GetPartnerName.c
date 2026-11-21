#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
extern s32 gWirelessPartnerNameValid;
extern u16 gWirelessPartnerName[];
extern UnkStruct_0207ad9c gWirelessSession;

s32 WirelessMgr_GetStatus(void);
u8 *WirelessMgr_GetChildInfo_0204324c(s32 a);
u8 *WirelessMgr_GetScanResult_020432e0(s32 a);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

#pragma thumb on
s32 WirelessSession_GetPartnerName(u16 *dst, s32 len) {
    u16 *src;

    if (gWirelessPartnerNameValid) {
        src = gWirelessPartnerName;
    } else if (WirelessMgr_GetStatus()) {
        if (gWirelessSession.unk0 == 0) {
            switch (gWirelessSession.unk2) {
            case 0:
            case 1:
                src = (u16 *)(WirelessMgr_GetChildInfo_0204324c(1) + 6);
                break;
            case 2:
            case 3:
                src = (u16 *)(WirelessMgr_GetScanResult_020432e0(0) + 0x50);
                break;
            default:
                src = gWirelessPartnerName;
                break;
            }
        } else {
            src = gWirelessPartnerName;
        }
    } else {
        src = gWirelessPartnerName;
    }
    if (len > 10)
        len = 10;
    MIi_CpuCopy16(src, dst, len * 2);
    if (len < src[10])
        return len;
    return src[10];
}
#pragma thumb reset
