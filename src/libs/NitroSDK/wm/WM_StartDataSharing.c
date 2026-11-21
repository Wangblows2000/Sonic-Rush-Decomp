#include <nitro/types.h>
#include <NitroSDK/wmds.h>
#include <NitroSDK/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
u32 MATH_CountPopulation(u32 x);
s32 WM_SetPortCallback(u16 port, void *cb, void *arg);
s32 WM_SetMPDataToPortEx(void *cb, void *arg, void *data, u16 size, u16 dest, u16 port, u16 prio);
void WmDataSharingReceiveCallback_Parent(void *cb);
void WmDataSharingSetDataCallback(void *cb);
void WmDataSharingReceiveCallback_Child(void *cb);

s32 WM_StartDataSharing(UnkDS *ds, u16 port, u16 aidBitmap, u16 dataLength, BOOL doubleMode)
{
    u16 childBitmap = 1;
    u16 aid;
    u8 *p = ((UnkWmWork *)WMi_GetSystemWork())->unk4;
    s32 r = WMi_CheckStateEx(2, 9, 10);
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(p + 0xc, 4);
    if (*(u32 *)(p + 0xc) == 0) {
        return 3;
    }
    if (ds == NULL) {
        return 6;
    }
    if (port >= 16) {
        return 6;
    }
    if (aidBitmap == 0) {
        return 6;
    }
    DC_InvalidateRange(p + 0x184, 2);
    aid = *(u16 *)(p + 0x184);
    if (aid == 0) {
        DC_InvalidateRange(p + 0x86, 2);
        childBitmap = *(u16 *)(p + 0x86);
    }
    {
        vu32 z = 0;
        MIi_CpuClearFast(z, ds, sizeof(UnkDS));
    }
    ds->writeIdx = 0;
    ds->readIdx = 0;
    ds->unk80c = 0;
    ds->dataLength = dataLength;
    ds->port = port;
    ds->aidBitmap = 0;
    ds->doubleMode = doubleMode ? 1 : 0;
    {
        u16 bm = aidBitmap | (1 << aid);
        u32 n;
        ds->aidBitmap = bm;
        n = MATH_CountPopulation(bm);
        ds->count = n;
        ds->setLength = dataLength * n;
    }
    if (ds->setLength > 0x1fc) {
        ds->aidBitmap = 0;
        return 6;
    }
    ds->setLength += 4;
    ds->state = 1;
    if (aid == 0) {
        s32 i;
        for (i = 0; i < 4; i++) {
            ds->buf[i].bitmap0 = ds->aidBitmap & (childBitmap | 1);
        }
        WM_SetPortCallback(port, WmDataSharingReceiveCallback_Parent, ds);
        for (i = 0; i < (ds->doubleMode == 1 ? 2 : 1); i++) {
            ds->writeIdx = (ds->writeIdx + 1) & 3;
            r = WM_SetMPDataToPortEx(WmDataSharingSetDataCallback, ds, &ds->buf[i], ds->setLength, ds->aidBitmap & childBitmap, ds->port,
                              1);
            if (r == 7) {
                ds->seq[i] = 0xffff;
                ds->readIdx = (ds->readIdx + 1) & 3;
            } else if (r != 0 && r != 2) {
                ds->state = 5;
                return 1;
            }
        }
    } else {
        ds->readIdx = 3;
        WM_SetPortCallback(port, WmDataSharingReceiveCallback_Child, ds);
    }
    return 0;
}
