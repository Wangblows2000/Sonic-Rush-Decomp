#include <nitro/types.h>
#include <NitroSDK/wmds.h>
#include <NitroSDK/wm.h>

void WmDataSharingReceiveCallback_Parent(void *cb);
void WmDataSharingReceiveCallback_Child(void *cb);
s32 WM_SetPortCallback(u16 port, void *cb, void *arg);

s32 WM_EndDataSharing(UnkDS *ds)
{
    u8 *sys = WMi_GetSystemWork();
    s32 r = WMi_CheckStateEx(2, 9, 10);
    void *portCb;
    if (r != 0) {
        return r;
    }
    if (ds == NULL) {
        return 6;
    }
    if (ds->aidBitmap == 0 ||
        ((portCb = ((void **)(sys + 0xc0))[ds->port]) != WmDataSharingReceiveCallback_Parent && portCb != WmDataSharingReceiveCallback_Child)) {
        return 3;
    }
    WM_SetPortCallback(ds->port, NULL, NULL);
    ds->aidBitmap = 0;
    ds->state = 0;
    return 0;
}
