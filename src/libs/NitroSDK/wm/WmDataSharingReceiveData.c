#include <nitro/types.h>
#include <NitroSDK/wmds.h>

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);

void WmDataSharingReceiveData(UnkDS *ds, u16 aid, const void *src)
{
    u16 bit = 1 << aid;
    u16 idx;
    void *data;
    u32 e;

    if (!(ds->aidBitmap & bit)) {
        return;
    }
    idx = ds->writeIdx;
    if (!(ds->buf[idx].bitmap0 & bit)) {
        if (ds->doubleMode != 1) {
            return;
        }
        idx = (idx + 1) & 3;
        if (!(ds->buf[idx].bitmap0 & bit)) {
            return;
        }
    }
    data = WmGetSharedDataAddress(ds, ds->aidBitmap, ds->buf[idx].data, aid);
    if (src != NULL) {
        MIi_CpuCopy16(src, data, ds->dataLength);
    } else {
        vu16 z = 0;
        MIi_CpuClear16(z, data, ds->dataLength);
    }
    e = OS_DisableInterrupts();
    ds->buf[idx].bitmap0 &= ~bit;
    ds->buf[idx].bitmap1 |= bit;
    OS_RestoreInterrupts(e);
}
