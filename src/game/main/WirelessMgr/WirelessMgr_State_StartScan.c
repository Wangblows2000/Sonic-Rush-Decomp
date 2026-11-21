#include <nitro/types.h>

typedef struct {
    u8 b[6];
} UnkMac_02041b00;

typedef struct {
    void (*fn)(void);
    u8 pad4[0x16];
    u16 unk1a;
} UnkWork_02041b00;

extern UnkMac_02041b00 data_02078b0c;
extern u8 gWirelessSsidData[];

u32 WH_GetSystemState(void);
void WH_SetSsid(void *p, u32 size);
void WH_StartScan(void (*fn)(void), UnkMac_02041b00 *mac, s32 a);
void WirelessMgr_OnScanFoundParent(void);
void WirelessMgr_State_Scan(void);

void WirelessMgr_State_StartScan(UnkWork_02041b00 *w) {
    UnkMac_02041b00 mac;

    if (WH_GetSystemState() != 1)
        return;
    mac = data_02078b0c;
    WH_SetSsid(gWirelessSsidData, 0x18);
    WH_StartScan(WirelessMgr_OnScanFoundParent, &mac, 0);
    w->unk1a = 0;
    w->fn = WirelessMgr_State_Scan;
}
