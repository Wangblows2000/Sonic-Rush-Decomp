#include <nitro/types.h>

extern u16 gWirelessSession[];
extern u16 data_022c44f8[];
extern u16 data_022c4500[];
void WirelessMgr_StartWfsParent(u16 *info, void *a, u16 b, u16 c, void *data, u16 len);
void WirelessMgr_StartWfsChild(u16 *info, u16 b, u16 c, void *data, u16 len);

#pragma thumb on
void WirelessSession_StartWfs(void) {
    gWirelessSession[0] = 1;
    switch (gWirelessSession[1]) {
    case 0:
    case 1:
        WirelessMgr_StartWfsParent(data_022c44f8, NULL, 0x200, 0x10, NULL, 0);
        break;
    case 2:
    case 3:
        WirelessMgr_StartWfsChild(data_022c4500, 0x200, 0x10, NULL, 0);
        break;
    }
}
#pragma thumb reset
