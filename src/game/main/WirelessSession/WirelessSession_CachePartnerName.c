#include <nitro/types.h>

#pragma thumb on

typedef struct { u8 unk0[0x14]; u16 unk14; } UnkStruct_022c4644;
extern UnkStruct_022c4644 gWirelessPartnerName;
extern u32 gWirelessPartnerNameValid;
u16 WirelessSession_GetPartnerName(void *p, s32 n);
void WirelessSession_CachePartnerName(void) { gWirelessPartnerName.unk14 = WirelessSession_GetPartnerName(&gWirelessPartnerName, 10); gWirelessPartnerNameValid = 1; }

#pragma thumb reset
